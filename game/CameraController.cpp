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
	SYSTEM::pBehavior_->AddBehavior(this);
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
		SYSTEM::pGraphic_->GetWidth() / float(SYSTEM::pGraphic_->GetHeight()),
		1.f
	);

	//mode_ = CAMERA_ORBIT;
	mode_ = CAMERA_FREE;
}

void CameraController::Update(float dt)
{
	if (mode_ == CAMERA_FREE)
		FreeMovingCamera(dt);
	else
		OrbitingCamera(dt);
}

void CameraController::Close() {}

void CameraController::Unload() {}

void CameraController::EditorUpdate(const float /*dt*/)
{
	// TODO
}

void CameraController::FreeMovingCamera(float dt)
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
			yaw = diff.x * m_camera->fovy / SYSTEM::pGraphic_->GetWidth();
			pitch = diff.y * m_camera->fovy / SYSTEM::pGraphic_->GetHeight();

			m_camera->Yaw(-yaw);
			m_camera->Pitch(pitch);
		}
		active = true;
	}
	else
		active = false;
	
	static float move = 150.f, zoom = 25.f;

	// move back and forward
	if (INPUT::KeyPressed(JE_W))
		m_camera->position -= move * dt * m_camera->GetBack();

	else if (INPUT::KeyPressed(JE_S))
		m_camera->position += move * dt * m_camera->GetBack();

	// move left and right
	if (INPUT::KeyPressed(JE_D))
		m_camera->position += move * dt * m_camera->GetRight();

	else if (INPUT::KeyPressed(JE_A))
		m_camera->position -= move * dt * m_camera->GetRight();

	// zoom in/out by scrolling mouse wheel
	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP))
		m_camera->fovy += zoom * dt;

	else if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN))
		m_camera->fovy -= zoom * dt;

	if (INPUT::KeyPressed(JE_MOUSE_MIDDLE)) {
		m_camera->position.Set(0, 0, 250);
		m_camera->fovy = 45.f;
	}

	// Update target as well
	m_camera->target = m_camera->position - m_camera->GetBack();
}

void CameraController::OrbitingCamera(float /*dt*/)
{
	static vec3 lastPosition, currentPosition = vec3::ZERO, 
		diff, newPosition;
	static float distance = 250.f;
	static float degreeVertical = 0.f, degreeHorizontal = 0.f;
	static bool active = false;

	newPosition.Set(m_camera->position);

	if (INPUT::KeyPressed(JE_MOUSE_LEFT)) {
		lastPosition = currentPosition;
		currentPosition = INPUT::GetOrhtoPosition();
		diff = lastPosition - currentPosition;
		if (active) {
			degreeVertical += diff.x * 180.f / SYSTEM::pGraphic_->GetWidth();
			degreeHorizontal += diff.y * 180.f / SYSTEM::pGraphic_->GetHeight();
		}
		active = true;
	}
	else
		active = false;

	// TODO
	// Degree to vector

	//float v_x = cosf(DegToRad(degreeVertical)), v_y = sinf(DegToRad(degreeVertical));
	//newPosition.x = distance * v_x;
	//newPosition.y = 0.f;
	//newPosition.z = distance * v_y;

	float h_x = cosf(DegToRad(degreeHorizontal)), h_y = sinf(DegToRad(degreeHorizontal));
	newPosition.x = 0.f;
	newPosition.y = distance * h_y;
	newPosition.z = distance * h_x;

	// 
	m_camera->position.Set(newPosition);
}

jeEnd
