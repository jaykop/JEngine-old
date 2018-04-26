#include "Telegram.h"
#include "MessageDispatcher.h"
#include "Object.h"
#include "StateManager.h"
#include "ObjectContainer.h"

JE_BEGIN

MessageDispatcher::Priority MessageDispatcher::m_messageQue;

void MessageDispatcher::Discharge(Object* _pReceiver, Telegram& msg)
{
	if (!_pReceiver->HandleMessage(msg))
		JE_DEBUG_PRINT("!MessageDispatcher - Message not handled!\n");
}

void MessageDispatcher::DispatcherMessage(float _delay, unsigned _sender, unsigned _receiver, const char* _MessageType, void *_extraInfo)
{
	Object* pReceiver = OBJECT::GetCurrentContainer()->GetObject(_receiver);
	Telegram telegram(0.f, _sender, _receiver, _MessageType, _extraInfo);

	if (_delay <= 0.f)
		Discharge(pReceiver, telegram);

	else {
		float currentTime = StateManager::GetCurrentTime();
		telegram.dispatchTime = currentTime + _delay;
		m_messageQue.insert(telegram);
	}
}


void MessageDispatcher::DispatchDelayedMessage()
{
	float currentTime = StateManager::GetCurrentTime();

	// Check if either there is que to execute
	while (!m_messageQue.empty() &&
		m_messageQue.begin()->dispatchTime < currentTime)
	{
		// Get telegram
		Telegram telegram = *m_messageQue.begin();
		// Get receiver
		Object* pReceiver = OBJECT::GetCurrentContainer()->GetObject(telegram.receiverId);

		// Operate event and remove the message from the list
		Discharge(pReceiver, telegram);
		m_messageQue.erase(m_messageQue.begin());
	}
}

JE_END