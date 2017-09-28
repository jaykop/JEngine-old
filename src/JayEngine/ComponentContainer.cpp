#include "Component.h"
#include "ComponentContainer.h"

NS_JE_BEGIN

void ComponentContainer::ClearContainer()
{
	// Clear all components in the list
	for (auto component : m_cptMap) {
		
		if (component.second) {
			delete component.second;
			component.second = nullptr;
		}
	}
}

NS_JE_END
