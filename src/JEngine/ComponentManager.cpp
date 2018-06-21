#include "Component.h"
#include "ComponentBuilder.h"
#include "ComponentManager.h"

jeBegin

BuilderMap	    COMPONENT::m_builderMap;
ComponentTypeMap    COMPONENT::m_typeMap,
                    COMPONENT::m_nameMap;
bool		    COMPONENT::m_loadingCustomLogic = true;

Component* ComponentManager::CreateComponent(
    const char* _componentName, Object* _pOwner)
{
    // Check if either there is a existing component builder 
    auto found = m_builderMap.find(_componentName);

    // If there is nothing like that,
    // return null
    if (found == m_builderMap.end()) {
        jeDebugPrint("!ComponentManager - No such name of enrolled component: %s\n", _componentName);
        return nullptr;
    }

#ifdef  jeUseBuiltInAllocator
    // Unless, return new component
    return found->second->CreateComponent(_pOwner, _componentName);
#else
    // Unless, return new component
    return found->second->CreateComponent(_pOwner);
#endif // jeUseBuiltInAllocator
}

#ifdef  jeUseBuiltInAllocator
void ComponentManager::RemoveComponent(Component* _component)
{
    // Check if either there is a existing component builder 
    auto found = m_builderMap.find(_component->m_typeName);
    found->second->RemoveComponent(_component);
}
#endif // jeUseBuiltInAllocator

const char* ComponentManager::KeyToTypeTranslator(const char* _name)
{
    auto found = m_typeMap.find(_name);
    if (found != m_typeMap.end())
        return found->second.c_str();

    return nullptr;
}

const char* ComponentManager::TypeToKeyTranslator(const char *_type)
{
    auto found = m_nameMap.find(_type);
    if (found != m_nameMap.end())
        return found->second.c_str();

    return nullptr;
}

void ComponentManager::ClearBuilders()
{
    // Delete instance
    for (auto it = m_builderMap.begin();
        it != m_builderMap.end(); )
        delete (it++)->second;

    // Clear nodes
    m_builderMap.clear();
    m_typeMap.clear();
}

jeEnd


