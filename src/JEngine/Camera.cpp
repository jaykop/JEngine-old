#include "Camera.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Camera);

Camera::Camera(Object* _pOwner)
	: Component(_pOwner),
	m_position(vec3::ZERO), m_up(vec3::UNIT_Y),
	m_target(vec3::ZERO), zoom(45.f)
{}

void Camera::Register()
{
	SystemManager::GetGraphicSystem()->AddCamera(this);
}

void Camera::operator=(const Camera & _copy)
{
	m_position.Set(_copy.m_position);
	m_up.Set(_copy.m_up);
	m_target.Set(_copy.m_target);
}

void Camera::Load(CR_RJValue _data)
{
	if (_data.HasMember("Up")) {
		CR_RJValue up = _data["Up"];
		m_up.Set(up[0].GetFloat(), up[1].GetFloat(), up[2].GetFloat());
	}

	if (_data.HasMember("Target")) {
		CR_RJValue target = _data["Target"];
		m_target.Set(target[0].GetFloat(), target[1].GetFloat(), target[2].GetFloat());
	}

	if (_data.HasMember("Position")) {
		CR_RJValue position = _data["Position"];
		m_position.Set(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
	}
}

void Camera::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

jeEnd
