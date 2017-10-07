#pragma once


NS_JE_BEGIN

class   Object;
class	Component;

class ComponentContainer {

public:

	friend class Object;



	void ClearContainer();

private:

	ComponentContainer() {};
	~ComponentContainer() {};

	ComponentContainer(const ComponentContainer& /*_copy*/) {};
	void operator=(const ComponentContainer& /*_copy*/) {};

	Object* m_pOwner;
	ComponentMap m_cptMap;

};

NS_JE_END

#include "ComponentContainer.inl"

