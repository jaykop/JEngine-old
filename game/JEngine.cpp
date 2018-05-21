#include "JEngine.h"
#include "Application.h"
#include "AssetManager.h"
#include "ComponentManager.h"
#include "CustomLogic.h"

jeUsingNamespace;

void JEngine::RegisterAssets()
{
	// Get assets ready 
	ASSET::SetInitDirectory("../resource/register/initData.json");
	ASSET::SetStateDirectory("../resource/register/state.json");
	ASSET::SetAssetDirectory("../resource/register/asset.json");
	ASSET::SetArchetypeDirectory("../resource/register/archetype.json");

	// Load customized components
	jeRegisterComponent(StateMachine);

	// States for bully
	jeRegisterComponent(BullyState);
	jeRegisterComponent(GoFight);
	jeRegisterComponent(JustHatingMiner);

	// States for miner
	jeRegisterComponent(MinerState);
	jeRegisterComponent(EnterMineAndDigForNugget);
	jeRegisterComponent(VisitBankAndDepositGold);
	jeRegisterComponent(QuenchThirst);
	jeRegisterComponent(GoHomeAndSleepTilRested);
	jeRegisterComponent(EatStew);
	jeRegisterComponent(BeatBully);

	// States for wife
	jeRegisterComponent(WifeState);
	jeRegisterComponent(DoHousework);
	jeRegisterComponent(CookStew);
	jeRegisterComponent(GoToBathroom);

	// Existing logics
	jeRegisterComponent(CameraController);
	jeRegisterComponent(LevelController);
	jeRegisterComponent(LightController);
	jeRegisterComponent(TestLogic);
}

void JEngine::Run(bool _imguiToggle)
{
	APP::Run(_imguiToggle);
}
