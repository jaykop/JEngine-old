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
	ComponentBuilder(const ComponentBuilder& /*_copy*/) = delete;
	void operator=(const ComponentBuilder& /*_copy*/) = delete;

};

JE_END