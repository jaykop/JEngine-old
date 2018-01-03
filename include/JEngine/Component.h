#pragma once
#include "Object.h"
#include "JsonParser.h"

JE_BEGIN

class Object;
class Component {

	friend class Object;
	friend class JsonParser;

public:

	unsigned GetOwnerId() { return m_ownerId; }

	virtual void Register() = 0;

protected:

	Component(Object* _owner, bool _byUser = false)
		: m_pOwner(_owner), 
		m_ownerId(_owner->GetId()), m_byUser(_byUser) {};
	virtual	~Component() {};

	virtual void Load(CR_RJValue _data) = 0;

	bool		m_byUser;
	Object*		m_pOwner;
	unsigned	m_ownerId;

private:

	Component() = delete;
	Component(const Component& /*_copy*/) = delete;
	void operator=(const Component& /*_copy*/) = delete;

};

JE_END
