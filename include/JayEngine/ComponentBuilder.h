#pragma once
#include "Macro.h"

JE_BEGIN

class Object;
class Component;

class ComponentBuilder {

public:

	virtual Component* CreateComponent(Object* _pOwner) const = 0;

protected:

	friend class AssetManager;
	friend class ComponentManager;

	ComponentBuilder() {};
	~ComponentBuilder() {};
	ComponentBuilder(const ComponentBuilder& /*_copy*/) {};
	void operator=(const ComponentBuilder& /*_copy*/) {};

};

JE_END