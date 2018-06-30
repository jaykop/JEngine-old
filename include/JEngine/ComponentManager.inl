#include <typeinfo>
#include "ComponentManager.h"

jeBegin

template <class ComponentType>
bool ComponentManager::RegisterBuilder(
    const char* componentName, ComponentBuilder* pBuilder)
{
    static const char* s_name;
    s_name = typeid(ComponentType).name();
    // Check if either there is a existing component builder 
    auto foundName = builderMap_.find(s_name);

    // If there is existing like that,
    // don't add new builder
    if (foundName != builderMap_.end()) {
        jeDebugPrint("!ComponentManager - No such name of enrolled component: %s\n", s_name);
        return false;
    }

    // Unless, add new builder
    else {
        builderMap_.insert(
            BuilderMap::value_type(s_name, pBuilder));

        // Key = Type name, Value = Class Name
        typeMap_.insert(
            ComponentTypeMap::value_type(componentName, typeid(ComponentType).name()));

        // Key = Class Name, Value = Type name
        nameMap_.insert(
            ComponentTypeMap::value_type(typeid(ComponentType).name(), componentName));

        if (loadingCustomLogic_)
            jeDebugPrint("*ComponentManager - Loaded custom logic: %s\n", componentName);

        return true;
    }

}

jeEnd
