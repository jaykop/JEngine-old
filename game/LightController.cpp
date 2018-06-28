#include "LightController.h"
#include "CustomLogicHeader.h"
#include "Light.h"

jeBegin

jeDefineCustomComponentBuilder(LightController);

LightController::LightController(Object* _pObject)
    :CustomComponent(_pObject)
{}

void LightController::Register()
{
    SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void LightController::Load(CR_RJValue /*_data*/)
{}

void LightController::Init()
{
    if (GetOwner()->HasComponent<Light>())
        m_light = GetOwner()->GetComponent<Light>();
}

void LightController::Update(float dt)
{
    float speed = 60.f * dt;
    if (INPUT::KeyPressed(JE_A))
        m_light->position.x -= speed;

    if (INPUT::KeyPressed(JE_D))
        m_light->position.x += speed;

    if (INPUT::KeyPressed(JE_W))
        m_light->position.y += speed;

    if (INPUT::KeyPressed(JE_S))
        m_light->position.y -= speed;

    if (INPUT::KeyPressed(JE_Z))
        m_light->position.z += speed;

    if (INPUT::KeyPressed(JE_C))
        m_light->position.z -= speed;
}

void LightController::Close()
{}

void LightController::Unload()
{}

void LightController::EditorUpdate(const float /*dt*/)
{
    // TODO
}

jeEnd
