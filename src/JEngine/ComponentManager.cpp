#include "ComponentBuilder.h"
#include "ComponentManager.h"

JE_BEGIN

BuilderMap			ComponentManager::m_builderMap;
ComponentTypeMap	ComponentManager::m_typeMap;
bool				ComponentManager::m_loadingCustomLogic = true;

Component* ComponentManager::CreateComponent(
	const char* _componentName, Object* _pOwner)
{
	// Check if either there is a existing component builder 
	auto found = m_builderMap.find(_componentName);

	// If there is nothing like that,
	// return null
	if (found == m_builderMap.end()) {
		JE_DEBUG_PRINT("!ComponentManager - No such name of enrolled component: %s\n", _componentName);
		return nullptr;
	}

	// Unless, return new component
	return found->second->CreateComponent(_pOwner);
}

const char* ComponentManager::KeyTranslator(const char* _name)
{
	auto found = m_typeMap.find(_name);
	if (found != m_typeMap.end())
		return found->second.c_str();

	return nullptr;
}

void ComponentManager::ClearBuilders()
{
	// Delete instance
	for (auto it = m_builderMap.begin();
		it != m_builderMap.end(); )
		delete ((it++)->second);
	
	// Clear nodes
	m_builderMap.clear();
	m_typeMap.clear();
}

JE_END


