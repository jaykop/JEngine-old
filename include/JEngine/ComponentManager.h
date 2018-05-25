#pragma once
#include "Macro.h"
#include <unordered_map>

jeBegin

class ComponentBuilder;
using ComponentTypeMap = std::unordered_map<std::string, std::string>;
using BuilderMap = std::unordered_map<std::string, ComponentBuilder*> ;

class Object;
class Component;
class ComponentBuilder;

class ComponentManager {

	friend class JEngine;
	friend class Object;
	friend class AssetManager;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(ComponentManager)

public:

	static const char* KeyToTypeTranslator(const char* _name);
	static const char* TypeToKeyTranslator(const char* _type);

	static Component*	CreateComponent(
		const char* _componentName, Object* _pOwner);

	static void	RemoveComponent(Component* _component);

private:

	static void	ClearBuilders();

	template <class ComponentType>
	static bool RegisterBuilder(
		const char* _componentName, ComponentBuilder* _pBuilder);

	static BuilderMap			m_builderMap;
	static ComponentTypeMap		m_typeMap, m_nameMap;
	static bool					m_loadingCustomLogic;
};

using COMPONENT = ComponentManager;

// Component manager macro
#define jeStringfy(x)					#x
#define jeConcat(a, b)					a ## b
#define jeRegisterComponent(c)			COMPONENT::RegisterBuilder<c>(jeStringfy(c), new jeConcat(c, Builder))
#define jeCheckComponentRegistration(c)	if (!(c)) { return false; } 

jeEnd

#include "ComponentManager.inl"
