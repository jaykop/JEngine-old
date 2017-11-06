#pragma once
#include "JsonParser.h"
#include "AssetManager.h"
#include "ComponentHeaders.h"
#include "ComponentManager.h"

JE_BEGIN

void AssetManager::RegisterComponent()
{
	// Load customized components
	JE_ADD_COMPONENT(SampleLogic);

	// Load in-built components
	JE_ADD_COMPONENT(Transform);
	JE_ADD_COMPONENT(Camera);
	JE_ADD_COMPONENT(Sprite);
	JE_ADD_COMPONENT(Light);
	// JE_ADD_COMPONENT(Model);
}

JE_END