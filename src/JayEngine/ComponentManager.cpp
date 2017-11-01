#include "ComponentBuilder.h"
#include "ComponentManager.h"

JE_BEGIN

BuilderMap ComponentManager::m_builderMap;

Component* ComponentManager::CreateComponent(
	const char* _componentName, Object* _pOwner)
{
	// Check if either there is a existing component builder 
	auto found = m_builderMap.find(_componentName);

	// If there is nothing like that,
	// return null
	if (found == m_builderMap.end()) {
		JE_DEBUG_PRINT("There is no such name of enrolled component.\n");
		return nullptr;
	}

	// Unless, return new component
	return found->second->CreateComponent(_pOwner);
}

void ComponentManager::RegisterBuilder(
	const char* _componentName, ComponentBuilder* _pBuilder)
{
	// Check if either there is a existing component builder 
	auto found = m_builderMap.find(_componentName);

	// If there is existing like that,
	// don't add new builder
	if (found != m_builderMap.end())
		JE_DEBUG_PRINT("There is a existing builder with same name.\n");

	// Unless, add new builder
	else
		m_builderMap.insert(
			BuilderMap::value_type(_componentName, _pBuilder));
}

void ComponentManager::ClearBuilders()
{
	// Delete instance
	for (auto it = m_builderMap.begin();
		it != m_builderMap.end(); ) 
		delete ((it++)->second);
	
	// Clear nodes
	m_builderMap.clear();
}

JE_END


