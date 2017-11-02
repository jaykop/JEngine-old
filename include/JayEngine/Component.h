#pragma once
#include "Macro.h"
#include "Object.h"
#include "JsonParser.h"

JE_BEGIN

class Object;
class Component {

public:

	friend class Object;
	unsigned GetOwnerId() { return m_ownerId; }

	virtual void Load(const RJValue& _data) = 0;
	virtual void Init() = 0;
	virtual void Update(float _dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;

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

JE_END
