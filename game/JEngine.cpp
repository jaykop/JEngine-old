#include "JEngine.h"
#include "Application.h"
#include "AssetManager.h"
#include "ComponentManager.h"
#include "CustomLogic.h"

USING_NS_JE;

void JEngine::RegisterAssets()
{
	// Get assets ready 
	ASSET::SetInitDirectory("../resource/register/initData.json");
	ASSET::SetStateDirectory("../resource/register/state.json");
	ASSET::SetAssetDirectory("../resource/register/asset.json");
	ASSET::SetArchetypeDirectory("../resource/register/archetype.json");

	// Load customized components
	JE_REGISTER_COMPONENT(StateMachine);

	// States for bully
	JE_REGISTER_COMPONENT(BullyState);
	JE_REGISTER_COMPONENT(GoFight);
	JE_REGISTER_COMPONENT(JustHatingMiner);

	// States for miner
	JE_REGISTER_COMPONENT(MinerState);
	JE_REGISTER_COMPONENT(EnterMineAndDigForNugget);
	JE_REGISTER_COMPONENT(VisitBankAndDepositGold);
	JE_REGISTER_COMPONENT(QuenchThirst);
	JE_REGISTER_COMPONENT(GoHomeAndSleepTilRested);
	JE_REGISTER_COMPONENT(EatStew);
	JE_REGISTER_COMPONENT(BeatBully);

	// States for wife
	JE_REGISTER_COMPONENT(WifeState);
	JE_REGISTER_COMPONENT(DoHousework); 
	JE_REGISTER_COMPONENT(CookStew);
	JE_REGISTER_COMPONENT(GoToBathroom);

	//Existing logics
	JE_REGISTER_COMPONENT(CameraController);
	JE_REGISTER_COMPONENT(LevelController);
	JE_REGISTER_COMPONENT(LightController);
	JE_REGISTER_COMPONENT(TestLogic);
}

void JEngine::Run(bool _imguiToggle)
{
	APP::Run(_imguiToggle);
}
