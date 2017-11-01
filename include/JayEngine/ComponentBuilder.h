#pragma once
#include "Macro.h"

JE_BEGIN

class Object;
class Component;

class ComponentBuilder {

	friend class ComponentManager;

public:

	virtual Component* CreateComponent(Object* _pOwner) const = 0;

private:

	ComponentBuilder() {};
	~ComponentBuilder() {};
	ComponentBuilder(const ComponentBuilder& /*_copy*/) {};
	void operator=(const ComponentBuilder& /*_copy*/) {};

};

JE_END