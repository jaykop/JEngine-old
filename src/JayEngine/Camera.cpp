#include "Camera.h"
#include "MathUtils.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

JE_BEGIN

Camera::Camera(Object* _owner)
	: Component(_owner),
	m_position(vec3::ZERO), m_up(vec3::UNIT_Y),
	m_target(vec3::ZERO)
{
	SystemManager::GetGraphicSystem()->AddCamera(this);
}

void Camera::Load(const RJValue& /*_data*/)
{
}

CameraBuilder::CameraBuilder()
	:ComponentBuilder()
{}

Component* CameraBuilder::CreateComponent(Object* _pOwner) const
{
	return new Camera(_pOwner);
}

JE_END
