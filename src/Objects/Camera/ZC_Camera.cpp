#include <ZC/Objects/Camera/ZC_Camera.h>

#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Events/ZC_Events.h>

ZC_Camera::ZC_Camera(const ZC_Vec3<float>& _camPos, const ZC_Vec3<float>& _lookOn, const ZC_Vec3<float>& _up,
        const ZC_Perspective& persp, const ZC_Ortho& ortho, bool useWindowSize, ZC_RenderLevel renderLevel)
    : ZC_View(&(uboSet.position), _lookOn, _up),
    ZC_Perspective(persp),
    ZC_Ortho(ortho),
    uboSet{ {}, {}, {}, _camPos },
    sConWindowResize(useWindowSize ? ZC_Events::ConnectWindowResize({ &ZC_Camera::WindowResize, this }) : ZC_EC())
{
    this->pOrtho = &(uboSet.ortho);

    static bool first = true;
    if (first)
    {
        first = false;
        upUbo = new ZC_UBO(ZC_UBO::Camera);
        upUbo->GLNamedBufferStorage(sizeof(uboSet), nullptr, GL_DYNAMIC_STORAGE_BIT);
        pActiveUBO = this + 1; // set in current ubo some random data, to set in update this object data and don't have in activeUBO nullptr
    }

    UboUpdate();

    ZC_UBOs::AddUpdateFunction(upUbo.Get(), { &ZC_Camera::UboUpdate, this }, renderLevel);
}

ZC_Camera::ZC_Camera(ZC_Camera&& c)
    : ZC_View(static_cast<ZC_View&&>(c)),
    ZC_Perspective(static_cast<ZC_Perspective&&>(c)),
    ZC_Ortho(static_cast<ZC_Ortho&&>(c)),
    uboSet(c.uboSet),
    sConWindowResize(c.sConWindowResize.IsConnected() ? ZC_Events::ConnectWindowResize({ &ZC_Camera::WindowResize, this }) : ZC_EC())
{
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
    bool perspNeedUpdate = this->PerspectiveUpdate(),
        viewNeedUpdate = this->ViewUpdate();
    if (perspNeedUpdate || viewNeedUpdate) uboSet.perspView = this->perspective * this->view;
    return &(uboSet.perspView);
}

const ZC_Mat4<float>* ZC_Camera::GetOrtho()
{
    this->OrthoUpdate();
    return &(uboSet.ortho);
}
#include <iostream>
void ZC_Camera::UboUpdate()
{
    bool perspNeedUpdate = this->PerspectiveUpdate(),
        viewNeedUpdate = this->ViewUpdate();
    bool perspViewNeedUpdate = perspNeedUpdate || viewNeedUpdate;
    if (perspViewNeedUpdate)
    {
        uboSet.perspView = this->perspective * this->view;
        // std::cout<<view[0][0]<<"   "<<view[1][0]<<"   "<<view[2][0]<<"   "<<view[3][0]<<std::endl;
        // std::cout<<view[0][1]<<"   "<<view[1][1]<<"   "<<view[2][1]<<"   "<<view[3][1]<<std::endl;
        // std::cout<<view[0][2]<<"   "<<view[1][2]<<"   "<<view[2][2]<<"   "<<view[3][2]<<std::endl;
        // std::cout<<view[0][3]<<"   "<<view[1][3]<<"   "<<view[2][3]<<"   "<<view[3][3]<<std::endl;
        // std::cout<<std::endl;
        // std::cout<<std::endl;


        // ZC_Mat4<float> view_skybox = this->view;
        // view_skybox[3][0] = 0.f;
        // view_skybox[3][1] = 0.f;
        // view_skybox[3][2] = 0.f;
        // uboSet.perspViewSkybox = this->perspective * view_skybox;
    }

    bool orthoNeedUpdate = this->OrthoUpdate();
    
    if (pActiveUBO != this)  //  last gpu update were from another camera, need update gpu
    {
        perspViewNeedUpdate = true;
        orthoNeedUpdate = true;
        pActiveUBO = this;
    }

    if (perspViewNeedUpdate && orthoNeedUpdate) upUbo->GLNamedBufferSubData(0, sizeof(uboSet), &uboSet);
    else if (perspViewNeedUpdate) upUbo->GLNamedBufferSubData(sizeof(uboSet.ortho), sizeof(uboSet.perspView) + sizeof(uboSet.position), &(uboSet.perspView));      //  if need update perspective view hight probability that camPos need too
    else if (orthoNeedUpdate) upUbo->GLNamedBufferSubData(0, sizeof(uboSet.ortho), &(uboSet.ortho));
}

void ZC_Camera::WindowResize(float width, float height)
{
    //  perspective
    this->SetAspect(width / height);
    //  ortho
    this->SetEndX(width);
    this->SetEndY(height);
}