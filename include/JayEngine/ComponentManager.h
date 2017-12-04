#pragma once
#include "Macro.h"
#include <unordered_map>

JE_BEGIN

class ComponentBuilder;
using BuilderMap = std::unordered_map<std::string, ComponentBuilder*> ;

class Object;
class Component;
class ComponentBuilder;

class ComponentManager {

	friend class Object;
	friend class AssetManager;

public:

	static void			RegisterBuilder(
		const char* _componentName, ComponentBuilder* _pBuilder);
	
private:

	static void			ClearBuilders();
	static Component*	CreateComponent(
		const char* _componentName, Object* _pOwner);

	static std::string m_nameHeader;
	static BuilderMap m_builderMap;

};

using COMPONENT = ComponentManager;

JE_END

// Component manager macro
#define JE_CONCAT(a, b) a ## b
#define JE_ADD_COMPONENT(c)	COMPONENT::RegisterBuilder(#c, new JE_CONCAT(c, Builder));