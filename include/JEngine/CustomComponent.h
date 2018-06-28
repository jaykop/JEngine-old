#pragma once
#include "Component.h"

jeBegin

struct Telegram;

class CustomComponent : public Component {

	friend class Object;
	friend class BehaviorSystem;

public:

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;

	virtual bool OnMessage(Telegram& msg) = 0;

protected:

	CustomComponent(Object* _pOwner)
		: Component(_pOwner, true) {};
	virtual	~CustomComponent() {};

private:

	CustomComponent() = delete;
	CustomComponent(const CustomComponent& /*_copy*/) = delete;
	void operator=(const CustomComponent& /*_copy*/) = delete;
};

jeEnd
