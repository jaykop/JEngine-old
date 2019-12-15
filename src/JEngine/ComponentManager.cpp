#include "Component.h"
#include "ComponentBuilder.h"
#include "ComponentManager.h"

jeBegin

BuilderMap			COMPONENT::builderMap_;
ComponentTypeMap    COMPONENT::typeMap_,
                    COMPONENT::nameMap_;
bool				COMPONENT::loadingCustomLogic_ = true;

Component* ComponentManager::CreateComponent(
    const char* componentName, Object* pOwner)
{
    // Check if either there is a existing component builder 
    auto found = builderMap_.find(componentName);

    // If there is nothing like that,
    // return null
    if (found == builderMap_.end()) {
        jeDebugPrint("!ComponentManager - No such name of enrolled component: %s\n", componentName);
        return nullptr;
    }

#ifdef  jeUseBuiltInAllocator
    // Unless, return new component
    return found->second->CreateComponent(pOwner, componentName);
#else
    // Unless, return new component
    return found->second->CreateComponent(pOwner);
#endif // jeUseBuiltInAllocator
}

#ifdef  jeUseBuiltInAllocator
void ComponentManager::RemoveComponent(Component* component)
{
    // Check if either there is a existing component builder 
    auto found = builderMap_.find(pComponent->typeName_);
    found->second->RemoveComponent(component);
}
#endif // jeUseBuiltInAllocator

const char* ComponentManager::KeyToTypeTranslator(const char* name)
{
    auto found = typeMap_.find(name);
    if (found != typeMap_.end())
        return found->second.c_str();

    return nullptr;
}

const char* ComponentManager::TypeToKeyTranslator(const char *type)
{
    auto found = nameMap_.find(type);
    if (found != nameMap_.end())
        return found->second.c_str();

    return nullptr;
}

void ComponentManager::ClearBuilders()
{
    // Delete instance
    for (auto it = builderMap_.begin();
        it != builderMap_.end(); )
        delete (it++)->second;

    // Clear nodes
    builderMap_.clear();
    typeMap_.clear();
}

jeEnd


