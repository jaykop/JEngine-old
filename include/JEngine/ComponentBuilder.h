#pragma once
#include "Macro.h"

jeBegin

class Object;
class Component;

class ComponentBuilder {

	friend class AssetManager;
	friend class ComponentManager;

	ComponentBuilder(ComponentBuilder&&) = delete;
	ComponentBuilder(const ComponentBuilder&) = delete;
	ComponentBuilder& operator=(ComponentBuilder&&) = delete;
	ComponentBuilder& operator=(const ComponentBuilder&) = delete;

public:

	virtual Component* CreateComponent(Object* _pOwner) const = 0;

protected:

	ComponentBuilder() {};
	~ComponentBuilder() {};

};

#define jeComponentBuilderDeclaration(c) class jeConcat(c, Builder) : public ComponentBuilder {	friend class JEngine; jeConcat(c, Builder)();	~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; CustomComponent* CreateComponent(Object* _pOwner) const override; };
#define jeComponentBuilderDefinition(c)	jeConcat(c, Builder)::jeConcat(c, Builder)() {} CustomComponent* jeConcat(c, Builder)::CreateComponent(Object* _pOwner) const {	return new (c)(_pOwner);}

jeEnd
