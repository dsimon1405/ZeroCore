#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Events/ZC_Events.h>

ZC_Camera::ZC_Camera(const ZC_Vec3<float>& _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up,
        const ZC_Perspective& persp, const ZC_Ortho& ortho, bool useWindowSize, ZC_RenderLevel renderLevel)
    : ZC_View(&(uboSet.position), _lookOn, _up),
    ZC_Perspective(persp),
    ZC_Ortho(ortho),
    uboSet{ {}, {}, _camPos },
    sConWindowResize(useWindowSize ? ZC_Events::ConnectWindowResize({ &ZC_Camera::WindowResize, this }) : ZC_EC())
{
    this->pOrtho = &(uboSet.ortho);

    static bool first = true;
    if (first)
    {
        first = false;
        ubo = new ZC_UBO(ZC_UBO::Camera);
        ubo->BufferData(sizeof(uboSet), nullptr, GL_DYNAMIC_DRAW);
        activeUBO = this + 1; // set in current ubo some random data, to set in update this object data and don't have in activeUBO nullptr
    }

    UboUpdate();

    ZC_UBOs::AddUpdateFunction(ubo.Get(), { &ZC_Camera::UboUpdate, this }, renderLevel);
}

ZC_Camera::ZC_Camera(ZC_Camera&& c)
    : ZC_View(dynamic_cast<ZC_View&&>(c)),
    ZC_Perspective(dynamic_cast<ZC_Perspective&&>(c)),
    ZC_Ortho(dynamic_cast<ZC_Ortho&&>(c)),
    uboSet(c.uboSet),
    sConWindowResize(c.sConWindowResize.IsConnected() ? ZC_Events::ConnectWindowResize({ &ZC_Camera::WindowResize, this }) : ZC_EC())
{
    c.sConWindowResize.Disconnect();
    if (activeCamera == &c) activeCamera = this;    
}

ZC_Camera::~ZC_Camera()
{
    if (activeCamera == this) activeCamera = nullptr;
    sConWindowResize.Disconnect();
}

void ZC_Camera::MakeActive()
{
    activeCamera = this;
}

ZC_Camera* ZC_Camera::GetActiveCamera()
{
    return activeCamera;
}

const ZC_Mat4<float>* ZC_Camera::GetPerspectiveView()
{
    bool perspNeedUpdate = this->PerspectiveUpdate(),
        viewNeedUpdate = this->ViewUpdate();
    if (perspNeedUpdate || viewNeedUpdate) uboSet.perspView = this->perspective * this->view;
    return &(uboSet.perspView);
}

void ZC_Camera::UboUpdate()
{
    bool perspNeedUpdate = this->PerspectiveUpdate(),
        viewNeedUpdate = this->ViewUpdate();
    bool perspViewNeedUpdate = perspNeedUpdate || viewNeedUpdate;
    if (perspViewNeedUpdate) uboSet.perspView = this->perspective * this->view;

    bool orthoNeedUpdate = this->OrthoUpdate();
    
    if (activeUBO != this)  //  last gpu update were from another camera, need update gpu
    {
        perspViewNeedUpdate = true;
        orthoNeedUpdate = true;
        activeUBO = this;
    }

    if (perspViewNeedUpdate && orthoNeedUpdate) ubo->BufferSubData(0, sizeof(uboSet), &uboSet);
    else if (perspViewNeedUpdate) ubo->BufferSubData(sizeof(uboSet.ortho), sizeof(uboSet.perspView) + sizeof(uboSet.position), &(uboSet.perspView));      //  if need update perspective view hight probability that camPos need too
    else if (orthoNeedUpdate) ubo->BufferSubData(0, sizeof(uboSet.ortho), &(uboSet.ortho));
}

void ZC_Camera::WindowResize(float width, float height)
{
    //  perspective
    this->SetAspect(width / height);
    //  ortho
    this->SetEndX(width);
    this->SetEndY(height);
}