#include "Component.h"
#include "ComponentContainer.h"

NS_JE_BEGIN

void ComponentContainer::ClearContainer()
{
	// Clear all components in the list
	for (auto it = m_cptMap.begin();
		it != m_cptMap.end(); ) {

		// Remove the one
		m_cptMap.erase(it);
		delete (it++)->second;
	}
}

NS_JE_END
