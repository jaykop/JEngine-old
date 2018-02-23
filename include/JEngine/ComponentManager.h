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

	friend class Core;
	friend class Object;
	friend class AssetManager;

public:

	static const char* KeyTranslator(const char* _name);
	static const char* TypeTranslator(const char* _type);

private:

	static void			ClearBuilders();
	static Component*	CreateComponent(
		const char* _componentName, Object* _pOwner);

	static BuilderMap			m_builderMap;
	static ComponentTypeMap		m_typeMap, m_nameMap;	
	static bool					m_loadingCustomLogic;

	template <class ComponentType>
	inline static bool RegisterBuilder(
		const char* _componentName, ComponentBuilder* _pBuilder);
};

using COMPONENT = ComponentManager;

JE_END

#include "ComponentManager.inl"

// Component manager macro
#define JE_STRINGFY(x) #x
#define JE_CONCAT(a, b) a ## b
#define JE_ADD_COMPONENT(c)	COMPONENT::RegisterBuilder<c>(JE_STRINGFY(c), new JE_CONCAT(c, Builder));