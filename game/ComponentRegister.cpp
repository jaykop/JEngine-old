#include "..\include\JayEngine\AssetManager.h"
#pragma once

#include "JsonParser.h"
#include "AssetManager.h"
#include "ComponentHeaders.h"
#include "ComponentManager.h"

JE_BEGIN

void AssetManager::RegisterComponent()
{
	// Load components
	JE_ADD_COMPONENT(SampleLogic);

	JE_ADD_COMPONENT(Transform);
	JE_ADD_COMPONENT(Camera);
	JE_ADD_COMPONENT(Sprite);
	JE_ADD_COMPONENT(Model);
	JE_ADD_COMPONENT(Light);
}

JE_END