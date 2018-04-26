#pragma once
#include "Component.h"

JE_BEGIN

struct Telegram;

class CustomComponent : public Component {

	friend class BehaviorSystem;

public:

	virtual void Init() = 0;
	virtual void Update(const float _dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;
	virtual bool OnMessage(Telegram& msg) = 0;

protected:

	CustomComponent(Object* _pOwner = nullptr)
		: Component(_pOwner, true) {};
	virtual	~CustomComponent() {};

private:

	CustomComponent() = delete;
	CustomComponent(const CustomComponent& /*_copy*/) = delete;
	void operator=(const CustomComponent& /*_copy*/) = delete;
};

JE_END
