#include "Camera.h"
#include "MathUtils.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

JE_BEGIN

Camera::Camera(Object* _owner)
	: Component(_owner),
	m_position(vec3::ZERO), m_up(vec3::UNIT_Y),
	m_target(vec3::ZERO)
{}

void Camera::Register()
{
	SystemManager::GetGraphicSystem()->AddCamera(this);
}

void Camera::Load(CR_RJValue _data)
{
	CR_RJValue up = _data["Up"];
	CR_RJValue target = _data["Target"];
	CR_RJValue position = _data["Position"];

	m_up.Set(up[0].GetFloat(), up[1].GetFloat(), up[2].GetFloat());
	m_target.Set(target[0].GetFloat(), target[1].GetFloat(), target[2].GetFloat());
	m_position.Set(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
}

CameraBuilder::CameraBuilder()
	:ComponentBuilder()
{}

Component* CameraBuilder::CreateComponent(Object* _pOwner) const
{
	return new Camera(_pOwner);
}

JE_END
