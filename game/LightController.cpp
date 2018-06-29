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
    SYSTEM::pBehavior_->AddBehavior(this);
}

void LightController::Load(CR_RJValue /*_data*/)
{}

void LightController::Init()
{
    if (GetOwner()->HasComponent<Light>())
        m_light = GetOwner()->GetComponent<Light>();

	if (GetOwner()->HasComponent<Transform>())
		transform_ = GetOwner()->GetComponent<Transform>();
}

void LightController::Update(float dt)
{
    float speed = 60.f * dt;
    if (INPUT::KeyPressed(JE_A))
		transform_->position.x -= speed;

    if (INPUT::KeyPressed(JE_D))
		transform_->position.x += speed;

    if (INPUT::KeyPressed(JE_W))
		transform_->position.y += speed;

    if (INPUT::KeyPressed(JE_S))
		transform_->position.y -= speed;

    if (INPUT::KeyPressed(JE_Z))
		transform_->position.z += speed;

    if (INPUT::KeyPressed(JE_C))
		transform_->position.z -= speed;
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
