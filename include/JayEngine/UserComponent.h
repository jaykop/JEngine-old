#pragma once
#include "Component.h"

JE_BEGIN

class UserComponent : public Component {

	friend class BehaviorSystem;

public:

	virtual void Init() = 0;
	virtual void Update(float _dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;

protected:

	UserComponent(Object* _owner = nullptr)
		: Component(_owner, true) {};
	virtual	~UserComponent() {};

private:

	UserComponent() = delete;
	UserComponent(const UserComponent& /*_copy*/) = delete;
	void operator=(const UserComponent& /*_copy*/) = delete;
};

JE_END
