#include "JEngine.h"
#include "Application.h"
#include "AssetManager.h"
#include "ComponentManager.h"
#include "CustomLogic.h"

jeUsingNamespace;

bool JEngine::RegisterAssets()
{
    // Get assets ready 
    ASSET::SetInitDirectory("../resource/register/initData.json");
    ASSET::SetStateDirectory("../resource/register/state.json");
    ASSET::SetAssetDirectory("../resource/register/asset.json");
    ASSET::SetArchetypeDirectory("../resource/register/archetype.json");

    // Load customized components
	jeCheckComponentRegistration(jeRegisterComponent(StateMachine));

    // States for bully
	jeCheckComponentRegistration(jeRegisterComponent(BullyState));
	jeCheckComponentRegistration(jeRegisterComponent(GoFight));
	jeCheckComponentRegistration(jeRegisterComponent(JustHatingMiner));

    // States for miner
	jeCheckComponentRegistration(jeRegisterComponent(MinerState));
	jeCheckComponentRegistration(jeRegisterComponent(EnterMineAndDigForNugget));
	jeCheckComponentRegistration(jeRegisterComponent(VisitBankAndDepositGold));
	jeCheckComponentRegistration(jeRegisterComponent(QuenchThirst));
	jeCheckComponentRegistration(jeRegisterComponent(GoHomeAndSleepTilRested));
	jeCheckComponentRegistration(jeRegisterComponent(EatStew));
	jeCheckComponentRegistration(jeRegisterComponent(BeatBully));

    // States for wife
	jeCheckComponentRegistration(jeRegisterComponent(WifeState));
	jeCheckComponentRegistration(jeRegisterComponent(DoHousework));
	jeCheckComponentRegistration(jeRegisterComponent(CookStew));
	jeCheckComponentRegistration(jeRegisterComponent(GoToBathroom));

    // Moving entity
	jeCheckComponentRegistration(jeRegisterComponent(Steering));

    // Existing logics
	jeCheckComponentRegistration(jeRegisterComponent(CameraController));
	jeCheckComponentRegistration(jeRegisterComponent(LevelController));
	jeCheckComponentRegistration(jeRegisterComponent(LightController));
	jeCheckComponentRegistration(jeRegisterComponent(TestLogic));

	return true;
}

void JEngine::CreateConsole()
{
	APP::CreateConsole();
}

void JEngine::CloseConsole()
{
	APP::CloseConsole();
}

void JEngine::Run(bool _imguiToggle)
{
    APP::Run(_imguiToggle);
}
