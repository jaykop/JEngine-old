NS_JE_BEGIN

template<typename ComponentType>
inline ComponentType* ComponentManager::CreateComponent(Object* _owner)
{
	// Check if component tpye is derived from base component
	if (std::is_base_of<Component, ComponentType>())
		return new ComponentType(_owner);

	return nullptr;
}

template<typename ComponentType>
inline void ComponentManager::RemoveComponent(ComponentType* _component)
{
	// Check if component tpye is derived from base component
	if (std::is_base_of<Component, ComponentType>()
		&& _component)
			delete _component;

	else
		JE_DEBUG_PRINT("Already deleted\n");
}

NS_JE_END