#include "JEngine.h"
#include "Application.h"
#include "AssetManager.h"
#include "ComponentManager.h"
#include "CustomLogic.h"

USING_NS_JE;

void JEngine::RegisterAssets()
{
	// Get assets ready 
	ASSET::SetInitDirectory("resource/register/initData.json");
	ASSET::SetStateDirectory("resource/register/state.json");
	ASSET::SetAssetDirectory("resource/register/asset.json");
	ASSET::SetArchetypeDirectory("resource/register/archetype.json");

	// Load customized components
	// States for bully
	JE_ADD_COMPONENT(BullyState);
	JE_ADD_COMPONENT(GoFight);
	JE_ADD_COMPONENT(JustHatingMiner);

	// States for miner
	JE_ADD_COMPONENT(MinerState);
	JE_ADD_COMPONENT(EnterMineAndDigForNugget);
	JE_ADD_COMPONENT(VisitBankAndDepositGold);
	JE_ADD_COMPONENT(QuenchThirst);
	JE_ADD_COMPONENT(GoHomeAndSleepTilRested);
	JE_ADD_COMPONENT(EatStew);
	JE_ADD_COMPONENT(BeatBully);

	// States for wife
	JE_ADD_COMPONENT(WifeState);
	JE_ADD_COMPONENT(DoHousework); 
	JE_ADD_COMPONENT(CookStew);
	JE_ADD_COMPONENT(GoToBathroom);

	//Existing logics
	JE_ADD_COMPONENT(CameraController);
	JE_ADD_COMPONENT(LevelController);
	JE_ADD_COMPONENT(LightController);
	JE_ADD_COMPONENT(TestLogic);
}

void JEngine::Run()
{
	APP::Run(true);	
}
