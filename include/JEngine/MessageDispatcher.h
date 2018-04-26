#pragma once
#include <set>
#include "Macro.h"
#include "Telegram.h"

JE_BEGIN

class Object;
class MessageDispatcher
{
public:
	static void DispatcherMessage(float _delay, unsigned _sender, 
		unsigned _receiver, const char* MessageType, void *extraInfo);
		
	static void DispatchDelayedMessage();

private:
	using Priority = std::set<Telegram>;
	Priority m_messageQue;

	static void Discharge(Object* _pReceiver, Telegram& msg);
};

JE_END