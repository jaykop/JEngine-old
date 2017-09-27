#include "ComponentManager.h"

NS_JE_BEGIN

template<typename ComponentType>
inline void ComponentContainer::AddComponent()
{
	// Insert new component to the list
	m_cptMap.insert(
		ComponentMap::value_type(typeid(ComponentType).name(),
			ComponentManager::CreateComponent<ComponentType>(m_owner)
			));
}

template<typename ComponentType>
inline ComponentType* ComponentContainer::GetComponent()
{
	// Find if there is the once
	auto found = m_cptMap.find(typeid(ComponentType).name());

	// If there is return it
	if (found != m_cptMap.end())
		return static_cast<ComponentType*>(found->second);

	// Unless...
	else {
		JE_DEBUG_PRINT("No such component\n");
		return nullptr;
	}
}

template<typename ComponentType>
inline bool ComponentContainer::HasComponent()
{
	// Find if there is the once
	auto found = m_cptMap.find(typeid(ComponentType).name());

	// If there is return it
	if (found != m_cptMap.end())
		return true;

	// Unless...
	else {
		JE_DEBUG_PRINT("No such component\n");
		return false;
	}
}


NS_JE_END