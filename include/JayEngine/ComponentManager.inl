#include "ComponentManager.h"

JE_BEGIN

template<typename ComponentType>
inline ComponentType* ComponentManager::CreateComponent(Object* _pOwner)
{
	// Check if component tpye is derived from base component
	if (std::is_base_of<Component, ComponentType>())
		return new ComponentType(_pOwner);

	return nullptr;
}

template<typename ComponentType>
inline void ComponentManager::RemoveComponent(ComponentType* _pComponent)
{
	// Check if component tpye is derived from base component
	if (std::is_base_of<Component, ComponentType>()
		&& _pComponent)
			delete _pComponent;

	else
		JE_DEBUG_PRINT("Already deleted\n");
}

JE_END
