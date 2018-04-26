#include "CameraController.h"
#include "GameLogicHeader.h"
#include "Camera.h"
#include "Vector4.h"

JE_BEGIN

CameraController::CameraController(Object* _pObject)
	:CustomComponent(_pObject), m_camera(nullptr),
	m_position(vec3::ZERO), m_target(vec3::ZERO)
{}

void CameraController::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void CameraController::Load(CR_RJValue /*_data*/)
{}

void CameraController::Init()
{
	m_camera = m_pOwner->GetComponent<Camera>();
	m_position = m_camera->m_position;
	m_camera->m_target = m_camera->m_position + vec3::UNIT_Z/*m_camera->m_position.GetNormalize()*/;
}

void CameraController::Update(const float _dt)
{
	static float s_speed = 5.f, s_speedByDt;
	static vec4 s_v4Target;
	static vec3 s_right, s_v3Target;
	s_v4Target.Set(m_camera->m_target.x, m_camera->m_target.y,
		m_camera->m_target.z, 1.f);
	s_v3Target.Set(m_camera->m_target);

	s_speedByDt = _dt * s_speed;

	if (INPUT::KeyPressed(JE_UP))
		m_position.z += s_speedByDt;

	if (INPUT::KeyPressed(JE_LEFT))
		m_position.x += s_speedByDt;

	if (INPUT::KeyPressed(JE_RIGHT))
		m_position.x -= s_speedByDt;

	if (INPUT::KeyPressed(JE_DOWN))
		m_position.z -= s_speedByDt;

	if (INPUT::KeyPressed(JE_A)) {
		s_v4Target = s_v4Target * mat4::Rotate(s_speedByDt, m_camera->m_up);
		s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
	}
	
	if (INPUT::KeyPressed(JE_D)){
		s_v4Target = s_v4Target * mat4::Rotate(-s_speedByDt, m_camera->m_up);
		s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
	}
	
	if (INPUT::KeyPressed(JE_W)) {
		s_right = m_camera->m_up.CrossProduct(s_v3Target.GetNormalize());
		s_v4Target.Set(s_v3Target.x, s_v3Target.y, s_v3Target.z, 1.f);
		s_v4Target = s_v4Target * mat4::Rotate(-s_speedByDt, s_right);
		s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
		m_camera->m_up = s_v3Target.CrossProduct(s_right.GetNormalize());
	}
	
	if (INPUT::KeyPressed(JE_S)) {
		s_right = m_camera->m_up.CrossProduct(s_v3Target.GetNormalize());
		s_v4Target.Set(s_v3Target.x, s_v3Target.y, s_v3Target.z, 1.f);
		s_v4Target = s_v4Target * mat4::Rotate(s_speedByDt, s_right);
		s_v3Target.Set(s_v4Target.x, s_v4Target.y, s_v4Target.z);
		m_camera->m_up = s_v3Target.CrossProduct(s_right.GetNormalize());
	}

	//vec3 newTarget = (s_v3Target - m_position).GetNormalize();
	m_camera->m_position = m_position;
	m_camera->m_target.Set(m_position - s_v3Target.GetNormalize());// (m_position + vec3::UNIT_Z);
}

void CameraController::Close()
{}

void CameraController::Unload()
{}

void CameraController::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

CameraControllerBuilder::CameraControllerBuilder()
	:ComponentBuilder()
{}

CustomComponent* CameraControllerBuilder::CreateComponent(Object* _pOwner) const
{
	return new CameraController(_pOwner);
}

JE_END