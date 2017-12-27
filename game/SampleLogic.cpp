#include "SampleLogic.h"
#include "GameLogicHeader.h"

#include "Camera.h"
#include "Particle.h"
#include "Vector4.h"
#include "GraphicSystem.h"

JE_BEGIN

SampleLogic::SampleLogic(Object* _pObject)
	:UserComponent(_pObject)
{}

void SampleLogic::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void SampleLogic::Load(CR_RJValue /*_data*/)
{}

void SampleLogic::Init()
{
	particle = STATE::m_pOBC->GetObject("Particle");
	m_moveSpeed = 150.f;
	released = true;
	m_camera = STATE::m_pOBC->GetObject("Camera")->GetComponent<Camera>();
	float width = float(SYSTEM::GetGraphicSystem()->GetWidth());
	m_widthSq = width*width;
}

void SampleLogic::Update(float _dt)
{
	cameraPos = m_camera->m_position;
	static float newDt = 0;
	newDt = _dt * m_moveSpeed;

	if (INPUT::KeyTriggered(JE_A)) {
		particle->GetComponent<Emitter>()->m_active
			= !particle->GetComponent<Emitter>()->m_active;
	}

	if (INPUT::KeyTriggered(JE_1))
		STATE::SetNextState("testState1");

	if (INPUT::KeyTriggered(JE_2))
		STATE::SetNextState("testState2");

	if (INPUT::KeyTriggered(JE_3))
		STATE::SetNextState("testState3");

	if (INPUT::KeyTriggered(JE_4))
		STATE::Pause("PauseState");

	if (INPUT::KeyTriggered(JE_5))
		STATE::Resume();

	if (INPUT::KeyTriggered(JE_6))
		STATE::ResumeAndNext("testState3");

	if (INPUT::KeyPressed(JE_MOUSE_LEFT)) {
		//if (released) {
		//	start = INPUT::m_orthoPosition;
		//	released = false;
		//}

		//current = INPUT::m_orthoPosition;

		//float startX_sq = start.x * start.x / m_widthSq, r_sq = cameraPos.z * cameraPos.z,
		//	currentX_sq = current.x * current.x / m_widthSq;

		//float y1 = sqrt(r_sq - startX_sq),
		//	y2 = sqrt(r_sq - currentX_sq);

		//static vec3 v1, v2, pos;
		//v1.Set(start.x, y1, 0.f);
		//v2.Set(current.x, y2, 0.f);
		//float angle = v2.GetAngle(v1);
		//pos = v1.GetRotated(angle, vec3::ZERO);
		//JE_DEBUG_PRINT("Angle: %f\n", angle);
		//m_camera->m_position.Set(pos.x, 0.f, pos.y);

		//JE_DEBUG_PRINT("x: %f, x: %f, x: %f\n", pos.x, pos.y, pos.z);
	}

	else
		released = true;

	if (INPUT::KeyTriggered(JE_MOUSE_RIGHT)) {
		;
	}

	if (INPUT::KeyTriggered(JE_MOUSE_MIDDLE))
		JE_DEBUG_PRINT("Middle Mouse\n");

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN)) {
		SYSTEM::GetGraphicSystem()->GetMainCamera()->m_position.z -= newDt;
		JE_DEBUG_PRINT("Mouse Wheel Down\n");
	}

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP)) {
		SYSTEM::GetGraphicSystem()->GetMainCamera()->m_position.z += newDt;
		JE_DEBUG_PRINT("Mouse Wheel Up\n");
	}

	if (INPUT::KeyPressed(JE_ESC)) {
		JE_DEBUG_PRINT("Quit\n");
		STATE::Quit();
	}
}

void SampleLogic::Close()
{}

void SampleLogic::Unload()
{}

SampleLogicBuilder::SampleLogicBuilder()
	:ComponentBuilder()
{}

UserComponent* SampleLogicBuilder::CreateComponent(Object* _pOwner) const
{
	return new SampleLogic(_pOwner);
}

JE_END