#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Events/ZC_Events.h>

ZC_Camera::ZC_Camera(const ZC_Vec3<float>& _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up,
        const ZC_Perspective& persp, const ZC_Ortho& ortho, bool useWindowSize, ZC_RenderLevel renderLevel)
    : ZC_View(&(uboSet.position), _lookOn, _up),
    ZC_Perspective(persp),
    ZC_Ortho(ortho),
    uboSet{ {}, {}, {}, {}, {}, _camPos },
    sConWindowResize(useWindowSize ? ZC_Events::ConnectWindowResize({ &ZC_Camera::WindowResize, this }) : ZC_EC())
{
    this->pOrtho = &(uboSet.ortho);
    this->pPerspective = &(uboSet.perspective);
    this->pView = &(uboSet.view);

    static bool first = true;
    if (first)
    {
        first = false;
        upUbo = new ZC_UBO(ZC_UBO_BP__Camera);
        upUbo->GLNamedBufferStorage(sizeof(uboSet), nullptr, GL_DYNAMIC_STORAGE_BIT);
        pActiveUBO = this + 1; // set in current ubo some random data, to set in update this object data and don't have in activeUBO nullptr
    }

    UboUpdate();

    ZC_UBOs::AddUpdateFunctionToUBOs(upUbo.Get(), { &ZC_Camera::UboUpdate, this }, renderLevel);
}

ZC_Camera::ZC_Camera(ZC_Camera&& c)
    : ZC_View(static_cast<ZC_View&&>(c)),
    ZC_Perspective(static_cast<ZC_Perspective&&>(c)),
    ZC_Ortho(static_cast<ZC_Ortho&&>(c)),
    uboSet(c.uboSet),
    sConWindowResize(c.sConWindowResize.IsConnected() ? ZC_Events::ConnectWindowResize({ &ZC_Camera::WindowResize, this }) : ZC_EC())
{
    this->pOrtho = &(uboSet.ortho);
    this->pPerspective = &(uboSet.perspective);
    this->pView = &(uboSet.view);

    c.sConWindowResize.Disconnect();
    if (pActiveCamera == &c) pActiveCamera = this;
}

ZC_Camera::~ZC_Camera()
{
    if (pActiveCamera == this) pActiveCamera = nullptr;
    sConWindowResize.Disconnect();
}

void ZC_Camera::MakeActive()
{
    pActiveCamera = this;
}

ZC_Camera* ZC_Camera::GetActiveCamera()
{
    return pActiveCamera;
}

const ZC_Mat4<float>* ZC_Camera::GetPerspectiveView()
{
    bool perspNeedUpdate = this->PerspectiveUpdate();
    bool viewNeedUpdate = this->ViewUpdate();
    if (perspNeedUpdate || viewNeedUpdate) uboSet.perspView = *(this->pPerspective) * *(this->pView);
    if (perspNeedUpdate) persp_need_update = true;
    if (viewNeedUpdate) view_need_update = true;
    return &(uboSet.perspView);
}

const ZC_Mat4<float>* ZC_Camera::GetOrtho()
{
    this->OrthoUpdate();
    return &(uboSet.ortho);
}

void ZC_Camera::UboUpdate()
{
    bool perspNeedUpdate = this->PerspectiveUpdate();
    bool viewNeedUpdate = this->ViewUpdate();
    if (perspNeedUpdate) persp_need_update = true;
    if (viewNeedUpdate) view_need_update = true;

    if (persp_need_update || view_need_update)
    {
        uboSet.perspView = *(this->pPerspective) * *(this->pView);

        ZC_Mat4<float> view_skybox = *(this->pView);
        view_skybox[3][0] = 0.f;
        view_skybox[3][1] = 0.f;
        view_skybox[3][2] = 0.f;
        uboSet.perspViewSkybox = *(this->pPerspective) * view_skybox;
    }

    bool orthoNeedUpdate = this->OrthoUpdate();
    
    if (pActiveUBO != this)  //  last gpu update were from another camera, need update gpu
    {
        orthoNeedUpdate = true;
        pActiveUBO = this;
    }

    if (orthoNeedUpdate && view_need_update) upUbo->GLNamedBufferSubData(0l, sizeof(uboSet), &uboSet);    //  ortho and view data divided with perspective data in UboSet, no sense to make 2 updates, update all
    else if (persp_need_update && view_need_update)
        upUbo->GLNamedBufferSubData(offsetof(UboSet, perspective), sizeof(UboSet) - sizeof(UboSet::ortho), &(uboSet.perspective));      //  update all among ortho. If need update perspective view hight probability that camPos need too
    else if (orthoNeedUpdate) upUbo->GLNamedBufferSubData(0l, sizeof(uboSet.ortho), &(uboSet.ortho));
    else if (persp_need_update) 
        upUbo->GLNamedBufferSubData(offsetof(UboSet, perspective), sizeof(uboSet.perspective) + sizeof(uboSet.perspView) + sizeof(uboSet.perspViewSkybox), &(uboSet.perspective));
    else upUbo->GLNamedBufferSubData(offsetof(UboSet, perspView), sizeof(UboSet) - offsetof(UboSet, perspView), &(uboSet.perspView));    //  remains view with cam pos

    persp_need_update = false;
    view_need_update = false;
}

void ZC_Camera::WindowResize(float width, float height)
{
    //  perspective
    this->SetAspect(width / height);
    //  ortho
    this->SetEndX(width);
    this->SetEndY(height);
}