#include "Object.h"
#include "ComponentManager.h"

JE_BEGIN

template<typename ComponentType>
inline void Object::AddComponent()
{
	const char* componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(componentName);

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == m_componentMap.end())
		m_componentMap.insert(
			ComponentMap::value_type(componentName,
				COMPONENT::CreateComponent<ComponentType>(this)
			));

	else
		JE_DEBUG_PRINT("*Object: Cannot add identical component again - %s\n", componentName);
}

template<typename ComponentType>
inline ComponentType* Object::GetComponent()
{
	// Find if there is the one
	const char* componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(componentName);

	// If there is return it
	if (found != m_componentMap.end())
		// dynamic_cast
		return static_cast<ComponentType*>(found->second);

	else {
		JE_DEBUG_PRINT("*Object: No such name of enrolled component - %s\n", componentName);
		return nullptr;
	}
}

template<typename ComponentType>
inline bool Object::HasComponent()
{
	// Find if there is the one
	const char* componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(componentName);

	// If there is return it
	if (found != m_componentMap.end())
		return true;

	else {
		JE_DEBUG_PRINT("*Object: No such name of enrolled component - %s\n", componentName);
		return false;
	}
}

template<typename ComponentType>
inline void Object::RemoveComponent()
{
	// Find if there is the one
	const char* componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(typeid(ComponentType).name());

	// If there is, remove it
	if (found != m_componentMap.end())
		delete found->second;

	else
		JE_DEBUG_PRINT("*Object: No such name of enrolled component - %s\n", componentName);
}

JE_END
