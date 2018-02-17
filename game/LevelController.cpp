#include "LevelController.h"
#include "GameLogicHeader.h"

JE_BEGIN

LevelController::LevelController(Object* _pObject)
	:CustomComponent(_pObject)
{}

void LevelController::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void LevelController::Load(CR_RJValue /*_data*/)
{}

void LevelController::Init()
{}

void LevelController::Update(const float /*_dt*/)
{

	if (INPUT::KeyTriggered(JE_1))
		STATE::SetNextState("level1");

	if (INPUT::KeyTriggered(JE_2))
		STATE::SetNextState("level2");

	if (INPUT::KeyTriggered(JE_3))
		STATE::SetNextState("level3");

	if (INPUT::KeyTriggered(JE_4))
		STATE::SetNextState("level4");

	if (INPUT::KeyTriggered(JE_5))
		STATE::SetNextState("level5");

	// TODO
	// No specific pause state
	//if (INPUT::KeyTriggered(JE_P))
	//{
	//	if (STATE::IsPaused())
	//		STATE::Resume();
	//	else
	//		STATE::Pause();
	//}

	if (INPUT::KeyTriggered(JE_R))
		STATE::Restart();

	if (INPUT::KeyPressed(JE_ESC)) {
		JE_DEBUG_PRINT("Quit\n");
		STATE::Quit();
	}
}

void LevelController::Close()
{}

void LevelController::Unload()
{}

LevelControllerBuilder::LevelControllerBuilder()
	:ComponentBuilder()
{}

CustomComponent* LevelControllerBuilder::CreateComponent(Object* _pOwner) const
{
	return new LevelController(_pOwner);
}

JE_END