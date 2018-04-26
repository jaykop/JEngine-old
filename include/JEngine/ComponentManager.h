#pragma once
#include "Macro.h"
#include <unordered_map>

JE_BEGIN

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

public:

	static const char* KeyTranslator(const char* _name);
	static const char* TypeTranslator(const char* _type);

	static Component*	CreateComponent(
		const char* _componentName, Object* _pOwner);

private:

	static void			ClearBuilders();

	template <class ComponentType>
	inline static void RegisterBuilder(
		const char* _componentName, ComponentBuilder* _pBuilder);

	static BuilderMap			m_builderMap;
	static ComponentTypeMap		m_typeMap, m_nameMap;
	static bool					m_loadingCustomLogic;
};

using COMPONENT = ComponentManager;

JE_END

#include "ComponentManager.inl"

// Component manager macro
#define JE_STRINGFY(x) #x
#define JE_CONCAT(a, b) a ## b
#define JE_ADD_COMPONENT(c)	COMPONENT::RegisterBuilder<c>(JE_STRINGFY(c), new JE_CONCAT(c, Builder))
#define JE_CREATE_COMPONENT(c, o) COMPONENT::CreateComponent(JE_STRINGFY(c), o)
#define JE_CREATE_CUSTOM_COMPONENT(c, o) (CustomComponent*)COMPONENT::CreateComponent(JE_STRINGFY(c), o)