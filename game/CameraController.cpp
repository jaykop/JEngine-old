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
}

void CameraController::Update(const float _dt)
{
	static vec3 lastPosition, currentPosition = vec3::ZERO, diff;
	static float yaw = 0.f, pitch = 0.f;
	static bool active = false;

	//lastPosition = currentPosition;
	//currentPosition = INPUT::GetOrhtoPosition();
	//diff = lastPosition - currentPosition;
	//if (active) {
	//	yaw = diff.x * 180.f / SYSTEM::GetGraphicSystem()->GetWidth();
	//	pitch = diff.y * 180.f / SYSTEM::GetGraphicSystem()->GetHeight();

	//	m_camera->Yaw(yaw);
	//	m_camera->Pitch(-pitch);
	//}
	//active = true;

	if (INPUT::KeyPressed(JE_MOUSE_LEFT)) {
		lastPosition = currentPosition;
		currentPosition = INPUT::GetOrhtoPosition();
		diff = lastPosition - currentPosition;
		if (active) {
			yaw = diff.x * m_camera->GetFovy() / SYSTEM::GetGraphicSystem()->GetWidth();
			pitch = diff.y * m_camera->GetFovy() / SYSTEM::GetGraphicSystem()->GetHeight();

			m_camera->Yaw(-yaw);
			m_camera->Pitch(pitch);
		}
		active = true;
	}
	else
		active = false;
	
	m_camera->target = m_camera->position - m_camera->GetBack();

	static float speed = 50.f;
	if (INPUT::KeyPressed(JE_W)) 
		m_camera->position -= speed *_dt * m_camera->GetBack();

	else if (INPUT::KeyPressed(JE_S)) 
		m_camera->position += speed * _dt * m_camera->GetBack();

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
