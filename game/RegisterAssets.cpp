#pragma once
#include "Core.h"
#include "AssetManager.h"
#include "ComponentManager.h"

/////////////////////////////////////////////////////////////////////////
// customize component headers
//////////////////////////////////////////////////////////////////////////
#include "CameraController.h"
#include "LevelController.h"
#include "LightController.h"

JE_BEGIN

void Core::Ready()
{
	// Get assets ready 
	ASSET::RegisterAssets();
}

void AssetManager::RegisterAssets()
{
	// Set directories
	ASSET::m_initDirectory.assign("resource/register/initData.json");
	ASSET::m_stateDirectory.assign("resource/register/state.json");
	ASSET::m_assetDirectory.assign("resource/register/asset.json");
	ASSET::m_archeDirectory.assign("resource/register/archetype.json");

	// Load customized components
	JE_ADD_COMPONENT(CameraController);
	JE_ADD_COMPONENT(LevelController);
	JE_ADD_COMPONENT(LightController);

}

JE_END