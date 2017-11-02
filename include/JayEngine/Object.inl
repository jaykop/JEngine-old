#include "Object.h"
#include "ComponentManager.h"

JE_BEGIN

template<typename ComponentType>
inline void Object::AddComponent()
{
	auto found = m_componentMap.find(typeid(ComponentType).name());

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == m_componentMap.end())
		m_componentMap.insert(
			ComponentMap::value_type(typeid(ComponentType).name(),
				COMPONENT::CreateComponent<ComponentType>(this)
			));

	else
		JE_DEBUG_PRINT("Cannot add identical component again!\n");
}

template<typename ComponentType>
inline ComponentType* Object::GetComponent()
{
	// Find if there is the one
	auto found = m_componentMap.find(typeid(ComponentType).name());

	// If there is return it
	if (found != m_componentMap.end())
		return dynamic_cast<ComponentType*>(found->second);

	else {
		JE_DEBUG_PRINT("No such component\n");
		return nullptr;
	}
}

template<typename ComponentType>
inline bool Object::HasComponent()
{
	// Find if there is the one
	auto found = m_componentMap.find(typeid(ComponentType).name());

	// If there is return it
	if (found != m_componentMap.end())
		return true;

	else {
		JE_DEBUG_PRINT("No such component\n");
		return false;
	}
}

template<typename ComponentType>
inline void Object::RemoveComponent()
{
	// Find if there is the one
	auto found = m_componentMap.find(typeid(ComponentType).name());

	// If there is, remove it
	if (found != m_componentMap.end())
		delete found->second;

	else
		JE_DEBUG_PRINT("No such component.\n");
}

JE_END
