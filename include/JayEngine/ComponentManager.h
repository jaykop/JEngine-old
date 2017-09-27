#pragma once
#include "Macro.h"

NS_JE_BEGIN

class Object;
class ComponentManager {

public:

	template <typename ComponentType>
	static inline ComponentType* CreateComponent(Object* _owner);

	template <typename ComponentType>
	static inline void RemoveComponent(ComponentType* _component);

private:

};

NS_JE_END

#include "ComponentManager.inl"

