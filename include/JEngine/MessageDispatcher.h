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

	static void DispatchMessage(float delay, unsigned sender,
		unsigned receiver, const char* messageType, void *extraInfo);
		
	static void DispatchDelayedMessage();

private:

	static Priority messageQue_;
	static void Discharge(Object* pReceiver, Telegram& msg);

	MessageDispatcher() = delete;
	~MessageDispatcher() = delete;
	MessageDispatcher(const MessageDispatcher& /*copy*/) = delete;
	MessageDispatcher& operator=(const MessageDispatcher& /*copy*/) = delete;
	MessageDispatcher(MessageDispatcher&& /*copy*/) = delete;
	MessageDispatcher& operator=(MessageDispatcher&& /*copy*/) = delete;
};

using DISPATCHER = MessageDispatcher;

jeEnd
