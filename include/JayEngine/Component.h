#pragma once
#include "Macro.h"

NS_JE_BEGIN

class Object;
class Component {

public:

	Component(Object* _owner)
		: m_pOwner(_owner) {};

	virtual	~Component() {};

private:

	Component() {};
	Component(const Component& /*copy*/) {};
	void operator=(const Component& /*copy*/) {};

	Object* m_pOwner;

};

NS_JE_END