#pragma once
#include "Macro.h"
#include <unordered_map>

NS_JE_BEGIN

class   Object;
class	Component;
typedef std::unordered_map<std::string, Component*>	ComponentMap;

class ComponentContainer {

public:

	friend class Object;

	template <typename ComponentType>
	inline void				AddComponent();
	
	template <typename ComponentType>
	inline ComponentType*	GetComponent();

	template <typename ComponentType>
	inline bool				HasComponent();

	void ClearContainer();

private:

	Object* m_owner;
	ComponentMap m_cptMap;

};

NS_JE_END

#include "ComponentContainer.inl"

