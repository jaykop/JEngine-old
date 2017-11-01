#pragma once

#include "JsonParser.h"
#include "ComponentHeaders.h"
#include "ComponentManager.h"

JE_BEGIN

void RegisterComponent()
{
	ComponentManager::RegisterBuilder();
}

JE_END