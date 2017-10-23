#pragma once
#include "Macro.h"

JE_BEGIN

class Object;
class ComponentManager {

public:

	template <typename ComponentType>
	static inline ComponentType* CreateComponent(Object* _pOwner);

	template <typename ComponentType>
	static inline void RemoveComponent(ComponentType* _pComponent);

private:

};

JE_END

#include "ComponentManager.inl"

