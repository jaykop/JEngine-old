#pragma once
#include <Macro.h>

NS_JE_BEGIN

class Object;

class Event {

public:

	//friend class EventHandler;
	
	Event() :
		m_owner(nullptr), m_eventId(0)
	{};
	~Event() {};

	virtual void Load() = 0;
	virtual void Init() = 0;
	virtual void Update(float _dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;

private:

	Event(const Event& /*copy*/) {};
	void operator=(const Event& /*copy*/) {};

	Object* m_owner;
	unsigned m_eventId;

};

NS_JE_END