#include "CameraController.h"
#include "CustomLogicHeader.h"
#include "Camera.h"
#include "MathUtils.h"

jeBegin
jeDefineCustomComponentBuilder(CameraController);

using namespace Math;

CameraController::CameraController(Object* _pObject)
	:CustomComponent(_pObject), m_camera(nullptr),
	position(vec3::ZERO), m_target(vec3::ZERO)
{}

void CameraController::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void CameraController::Load(CR_RJValue /*_data*/)
{}

void CameraController::Init()
{
	m_camera = GetOwner()->GetComponent<Camera>();
	position = m_camera->position;
	//m_camera->target = m_camera->position + vec3::UNIT_Z/*m_camera->position.GetNormalize()*/;
}

void CameraController::Update(const float _dt)
{
	//static float s_speed = 5.f, s_speedByDt;
	//static vec4 s_v4Target;
	//static vec3 s_right, s_v3Target;
	//s_v4Target.Set(m_camera->target.x, m_camera->target.y,
	//	m_camera->target.z, 1.f);
	//s_v3Target.Set(m_camera->target);

	//s_speedByDt = _dt * s_speed;

	//if (INPUT::KeyPressed(JE_UP))
	//	position.z += s_speedByDt;

	//if (INPUT::KeyPressed(JE_LEFT))
	//	position.x += s_speedByDt;

	//if (INPUT::KeyPressed(JE_RIGHT))
	//	position.x -= s_speedByDt;

	//if (INPUT::KeyPressed(JE_DOWN))
	//	position.z -= s_speedByDt;

	//if (INPUT::KeyPressed(JE_A)) {
	//	s_v4Target = s_v4Target * Rotate(s_speedByDt, m_camera->up);
	//	s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
	//}
	//
	//if (INPUT::KeyPressed(JE_D)){
	//	s_v4Target = s_v4Target * Rotate(-s_speedByDt, m_camera->up);
	//	s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
	//}
	//
	//if (INPUT::KeyPressed(JE_W)) {
	///*	s_right = CrossProduct(m_camera->up, GetNormalize(s_v3Target));
	//	s_v4Target.Set(s_v3Target.x, s_v3Target.y, s_v3Target.z, 1.f);
	//	s_v4Target = s_v4Target * Rotate(-s_speedByDt, s_right);
	//	s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
	//	m_camera->up = CrossProduct(s_v3Target, GetNormalize(s_right));*/
	//	m_camera->position += _dt * m_camera->;
	//}
	//
	//if (INPUT::KeyPressed(JE_S)) {
	//	s_right = CrossProduct(m_camera->up, GetNormalize(s_v3Target));
	//	s_v4Target.Set(s_v3Target.x, s_v3Target.y, s_v3Target.z, 1.f);
	//	s_v4Target = s_v4Target * Rotate(s_speedByDt, s_right);
	//	s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
	//	m_camera->up = CrossProduct(s_v3Target, GetNormalize(s_right));
	//}

	////vec3 newTarget = (s_v3Target - position).GetNormalize();
	//m_camera->position = position;
	//m_camera->target.Set(position - GetNormalize(s_v3Target));// (position + vec3::UNIT_Z);
}

void CameraController::Close()
{}

void CameraController::Unload()
{}

void CameraController::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

jeEnd
