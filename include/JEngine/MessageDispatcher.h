#pragma once
#include <set>
#include "Macro.h"

JE_BEGIN

class	Object;
struct	Telegram;

class MessageDispatcher
{

	using Priority = std::set<Telegram>;

public:

	static void DispatcherMessage(float _delay, unsigned _sender, 
		unsigned _receiver, const char* _MessageType, void *_extraInfo);
		
	static void DispatchDelayedMessage();

private:

	static Priority m_messageQue;
	static void Discharge(Object* _pReceiver, Telegram& msg);
};

using DISPATCHER = MessageDispatcher;

JE_END