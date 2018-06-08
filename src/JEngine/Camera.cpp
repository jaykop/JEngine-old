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
	position(vec3::ZERO), up(vec3::UNIT_Y),
	target(vec3::ZERO), zoom(45.f)
{}

void Camera::Register()
{
	SystemManager::GetGraphicSystem()->AddCamera(this);
}

void Camera::operator=(const Camera & _copy)
{
	position.Set(_copy.position);
	up.Set(_copy.up);
	target.Set(_copy.target);
}

void Camera::Load(CR_RJValue _data)
{
	if (_data.HasMember("Up")) {
		CR_RJValue loadedUp = _data["Up"];
		up.Set(loadedUp[0].GetFloat(), loadedUp[1].GetFloat(), loadedUp[2].GetFloat());
	}

	if (_data.HasMember("Target")) {
		CR_RJValue loadedTarget = _data["Target"];
		target.Set(loadedTarget[0].GetFloat(), loadedTarget[1].GetFloat(), loadedTarget[2].GetFloat());
	}

	if (_data.HasMember("Position")) {
		CR_RJValue loadedPosition = _data["Position"];
		position.Set(loadedPosition[0].GetFloat(), loadedPosition[1].GetFloat(), loadedPosition[2].GetFloat());
	}
}

void Camera::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

jeEnd
