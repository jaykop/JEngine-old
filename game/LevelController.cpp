#include "LevelController.h"
#include "CustomLogicHeader.h"

jeBegin

jeDefineCustomComponentBuilder(LevelController);

LevelController::LevelController(Object* _pObject)
    :CustomComponent(_pObject)
{}

void LevelController::Register()
{
    SYSTEM::pBehavior_->AddBehavior(this);
}

void LevelController::Load(CR_RJValue /*_data*/)
{}

void LevelController::Init()
{}

void LevelController::Update(const float /*dt*/)
{

    //if (INPUT::KeyTriggered(JE_1))
    //	STATE::SetNextState("level1");

    //if (INPUT::KeyTriggered(JE_2))
    //	STATE::SetNextState("level2");

    //if (INPUT::KeyTriggered(JE_3))
    //	STATE::SetNextState("level3");

    //if (INPUT::KeyTriggered(JE_4))
    //	STATE::SetNextState("level4");

    //if (INPUT::KeyTriggered(JE_5))
    //	STATE::SetNextState("level5");

    // TODO
    // No specific pause state
    //if (INPUT::KeyTriggered(JE_P))
    //{
    //	if (STATE::IsPaused())
    //		STATE::Resume();
    //	else
    //		STATE::Pause();
    //}

	if (INPUT::KeyTriggered(JE_SPACE))
		ASSET::TakeAScreenshot("../resource/screenshot/");

    if (INPUT::KeyTriggered(JE_R))
        STATE::Restart();

    if (INPUT::KeyTriggered(JE_ESC)) {
        jeDebugPrint("Quit\n");
        STATE::Quit();
    }
}

void LevelController::Close()
{}

void LevelController::Unload()
{}

void LevelController::EditorUpdate(const float /*dt*/)
{
    // TODO
}

jeEnd
