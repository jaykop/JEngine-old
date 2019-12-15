#pragma once
#include "Macro.h"
#include <unordered_map>

jeBegin

class ComponentBuilder;
using ComponentTypeMap = std::unordered_map<std::string, std::string>;
using BuilderMap = std::unordered_map<std::string, ComponentBuilder*>;

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

    static const char* KeyToTypeTranslator(const char* name);
    static const char* TypeToKeyTranslator(const char* type);

    static Component*	CreateComponent(
        const char* componentName, Object* pOwner);

#ifdef  jeUseBuiltInAllocator
    static void	RemoveComponent(Component* component);
#endif // jeUseBuiltInAllocator

private:

    static void	ClearBuilders();

    template <class ComponentType>
    static bool RegisterBuilder(
        const char* componentName, ComponentBuilder* pBuilder);

    static BuilderMap	    builderMap_;
    static ComponentTypeMap typeMap_, nameMap_;
    static bool				loadingCustomLogic_;
};

using COMPONENT = ComponentManager;

// Component manager macro
#define jeStringfy(x)					#x
#define jeConcat(a, b)					a ## b
#define jeRegisterComponent(c)			COMPONENT::RegisterBuilder<c>(jeStringfy(c), new jeConcat(c, Builder))
#define jeCheckComponentRegistration(c)	if (!(c)) { return false; } 

jeEnd

#include "ComponentManager.inl"
