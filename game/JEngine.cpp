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
	JE_ADD_COMPONENT(CameraController);
	JE_ADD_COMPONENT(LevelController);
	JE_ADD_COMPONENT(LightController);
	JE_ADD_COMPONENT(TestLogic);
}

void JEngine::Run()
{
	APP::Run(true);	
}
