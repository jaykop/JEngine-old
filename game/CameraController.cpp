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
	static vec3 firstPosition, secondPosition, diff;
	static bool clicked = false;

	if (INPUT::KeyPressed(JE_MOUSE_LEFT) && !clicked) {
		firstPosition = INPUT::GetOrhtoPosition();
		std::cout << firstPosition << std::endl;
		clicked = true;
	}

	else if (!INPUT::KeyPressed(JE_MOUSE_LEFT) && clicked) {
		secondPosition = INPUT::GetOrhtoPosition();
		diff = firstPosition - secondPosition;
		std::cout << secondPosition << std::endl;	
		
		float yaw = diff.x * m_camera->GetFovy() / SYSTEM::GetGraphicSystem()->GetWidth();
		float pitch = diff.y * m_camera->GetFovy() / SYSTEM::GetGraphicSystem()->GetHeight();

		jeDebugPrint("%f %f\n", yaw, pitch);

		m_camera->Yaw(yaw);
		m_camera->Pitch(pitch);
		clicked = false;
	}
	
	m_camera->target = m_camera->position - m_camera->GetBack();

	static float speed = 100.f;
	if (INPUT::KeyPressed(JE_W)) 
		m_camera->position += speed *_dt * m_camera->GetBack();

	else if (INPUT::KeyPressed(JE_S)) 
		m_camera->position -= speed * _dt * m_camera->GetBack();
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
