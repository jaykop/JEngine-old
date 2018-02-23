#include <typeinfo>
#include "ComponentManager.h"
#include "ImguiManager.h"

JE_BEGIN

template <class ComponentType>
bool ComponentManager::RegisterBuilder(
	const char* _componentName, ComponentBuilder* _pBuilder)
{
	static const char* s_name;
	s_name = typeid(ComponentType).name();
	// Check if either there is a existing component builder 
	auto foundName = m_builderMap.find(s_name);

	// If there is existing like that,
	// don't add new builder
	if (foundName != m_builderMap.end()) {
		JE_DEBUG_PRINT("!ComponentManager - No such name of enrolled component: %s\n", s_name);
		return false;
	}
	// Unless, add new builder
	else {
		m_builderMap.insert(
			BuilderMap::value_type(s_name, _pBuilder));

		// Key = Type name, Value = Class Name
		m_typeMap.insert(
			ComponentTypeMap::value_type(_componentName, typeid(ComponentType).name()));

		// Key = Class Name, Value = Type name
		m_nameMap.insert(
			ComponentTypeMap::value_type(typeid(ComponentType).name(), _componentName));

		if (m_loadingCustomLogic)
			JE_DEBUG_PRINT("*ComponentManager - Loaded custom logic: %s\n", _componentName);

		IMGUI::AddEditorFunc(ComponentType::EditorUpdate);

		return true;
	}

}

JE_END