#include "SampleLogic.h"
#include "GameLogicHeader.h"

JE_BEGIN

SampleLogic::SampleLogic(Object* _pObject)
	:Component(_pObject)
{}

void SampleLogic::Load(const RJValue& /*_data*/)
{}

void SampleLogic::Init()
{}

void SampleLogic::Update(float /*_dt*/)
{
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

	if (INPUT::KeyTriggered(JE_MOUSE_LEFT))
		JE_DEBUG_PRINT("Left Mouse\n");

	if (INPUT::KeyTriggered(JE_MOUSE_RIGHT))
		JE_DEBUG_PRINT("Right Mouse\n");

	if (INPUT::KeyTriggered(JE_MOUSE_MIDDLE))
		JE_DEBUG_PRINT("Middle Mouse\n");

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN))
		JE_DEBUG_PRINT("Mouse Wheel Down\n");

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP))
		JE_DEBUG_PRINT("Mouse Wheel Up\n");

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

Component* SampleLogicBuilder::CreateComponent(Object* _pOwner) const
{
	return new SampleLogic(_pOwner);
}

JE_END