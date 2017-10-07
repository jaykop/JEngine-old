#pragma once
#include "Macro.h"
#include "Object.h"

NS_JE_BEGIN

class Object;
class Component {

public:

	friend class Object;

	Component() {};
	Component(Object* _owner)
		: m_pOwner(_owner), m_id(_owner->GetId()) {};

	virtual	~Component() {};

protected:

	Object* m_pOwner;
	unsigned m_id;

private:

	Component(const Component& /*_copy*/) {};
	void operator=(const Component& /*_copy*/) {};

};

NS_JE_END