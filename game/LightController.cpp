#include "LightController.h"
#include "CustomLogicHeader.h"
#include "Light.h"

jeBegin

jeDefineCustomComponentBuilder(LightController);

LightController::LightController(Object* pObject)
    :CustomComponent(pObject)
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
		transform_->position_.x -= speed;

    if (INPUT::KeyPressed(JE_D))
		transform_->position_.x += speed;

    if (INPUT::KeyPressed(JE_W))
		transform_->position_.y += speed;

    if (INPUT::KeyPressed(JE_S))
		transform_->position_.y -= speed;

    if (INPUT::KeyPressed(JE_Z))
		transform_->position_.z += speed;

    if (INPUT::KeyPressed(JE_C))
		transform_->position_.z -= speed;
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
