#pragma once
#include "Macro.h"
#include <unordered_map>

JE_BEGIN

class ComponentBuilder;
typedef std::unordered_map<std::string, ComponentBuilder*> BuilderMap;

class Object;
class Component;
class ComponentManager {

public:

	static Component*	CreateComponent(
		const char* _componentName, Object* _pOwner);

	static void			RegisterBuilder(
		const char* _componentName, ComponentBuilder* _pBuilder);

	static void			ClearBuilders();

private:

	static BuilderMap m_builderMap;

};

JE_END


