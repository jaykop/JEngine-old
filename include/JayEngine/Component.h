#pragma once
#include "Macro.h"

NS_JE_BEGIN

class Object;
class Component {

public:

	Component() {};
	Component(Object* _owner)
		: m_pOwner(_owner) {};

	virtual	~Component() {};

private:

	Component(const Component& /*_copy*/) {};
	void operator=(const Component& /*_copy*/) {};

	Object* m_pOwner;

};

NS_JE_END