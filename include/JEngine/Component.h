#pragma once
#include "Object.h"
#include "JsonParser.h"

JE_BEGIN

class Object;
class Component {

	friend class Object;
	friend class JsonParser;

public:

	unsigned GetOwnerId() { return m_pOwnerId; }
	virtual void Register() = 0;

protected:

	Component(Object* _pOwner, bool _byUser = false)
		: m_pOwner(_pOwner), m_pOwnerId(_pOwner->GetId()), 
		m_byUser(_byUser), m_showWindow(false) {};
	virtual	~Component() {};
	void operator=(const Component& _copy) {
		m_byUser = _copy.m_byUser;
	};

	virtual void Load(CR_RJValue _data) = 0;

	bool		m_byUser, m_showWindow;
	Object*		m_pOwner;
	unsigned	m_pOwnerId;

private:

	Component() = delete;
	Component(const Component& /*_copy*/) = delete;

};

JE_END
