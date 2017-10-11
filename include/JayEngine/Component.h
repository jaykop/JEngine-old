#pragma once
#include "Macro.h"
#include "Object.h"

NS_JE_BEGIN

class Object;
class Component {

public:

	friend class Object;
	unsigned GetOwnerId() { return m_ownerId; }

protected:

	Component(Object* _owner = nullptr)
		: m_pOwner(_owner), m_ownerId(_owner->GetId()) {};
	virtual	~Component() {};

	Object* m_pOwner;
	unsigned m_ownerId;

private:

	Component(const Component& /*_copy*/) {};
	void operator=(const Component& /*_copy*/) {};

};

NS_JE_END