#include "CameraController.h"
#include "CustomLogicHeader.h"
#include "Camera.h"
#include "MathUtils.h"

jeBegin
jeDefineCustomComponentBuilder(CameraController);

using namespace Math;

CameraController::CameraController(Object* _pObject)
	:CustomComponent(_pObject), m_camera(nullptr)
{}

void CameraController::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void CameraController::Load(CR_RJValue /*_data*/) {}

void CameraController::Init()
{
	m_camera = GetOwner()->GetComponent<Camera>();
	m_camera->SetCamera(
		m_camera->position,
		-vec3::UNIT_Z,
		m_camera->GetUp(),
		45.f,
		GRAPHIC->GetWidth() / float(GRAPHIC->GetHeight()),
		1.f
	);
}

void CameraController::Update(float dt)
{
	static vec3 lastPosition, currentPosition = vec3::ZERO, diff;
	static float yaw = 0.f, pitch = 0.f;
	static bool active = false;

	// rotate the camera
	if (INPUT::KeyPressed(JE_MOUSE_LEFT)) {
		lastPosition = currentPosition;
		currentPosition = INPUT::GetOrhtoPosition();
		diff = lastPosition - currentPosition;
		if (active) {
			yaw = diff.x * m_camera->fovy / GRAPHIC->GetWidth();
			pitch = diff.y * m_camera->fovy / GRAPHIC->GetHeight();

			m_camera->Yaw(-yaw);
			m_camera->Pitch(pitch);
		}
		active = true;
	}
	else
		active = false;
	
	static float speed = 150.f;
	
	// move back and forward
	if (INPUT::KeyPressed(JE_W)) 
		m_camera->position -= speed * dt * m_camera->GetBack();

	else if (INPUT::KeyPressed(JE_S)) 
		m_camera->position += speed * dt * m_camera->GetBack();

	// move left and right
	if (INPUT::KeyPressed(JE_D))
		m_camera->position += speed * dt * m_camera->GetRight();

	else if (INPUT::KeyPressed(JE_A))
		m_camera->position -= speed * dt * m_camera->GetRight();

	// zoom in/out by scrolling mouse wheel
	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP))
		m_camera->fovy += dt;

	else if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN))
		m_camera->fovy -= dt;

	// Update target as well
	m_camera->target = m_camera->position - m_camera->GetBack();

}

void CameraController::Close() {}

void CameraController::Unload() {}

void CameraController::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
