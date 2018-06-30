#pragma once
#include <set>
#include "Macro.h"

jeBegin

class	Object;
struct	Telegram;

class MessageDispatcher
{

	using Priority = std::set<Telegram>;

public:

	static void DispatchMessage(float _delay, unsigned _sender,
		unsigned _receiver, const char* _MessageType, void *_extraInfo);
		
	static void DispatchDelayedMessage();

private:

	static Priority m_messageQue;
	static void Discharge(Object* _pReceiver, Telegram& msg);

	MessageDispatcher() = delete;
	~MessageDispatcher() = delete;
	MessageDispatcher(const MessageDispatcher& /*copy*/) = delete;
	MessageDispatcher& operator=(const MessageDispatcher& /*copy*/) = delete;
	MessageDispatcher(MessageDispatcher&& /*copy*/) = delete;
	MessageDispatcher& operator=(MessageDispatcher&& /*copy*/) = delete;
};

using DISPATCHER = MessageDispatcher;

jeEnd
