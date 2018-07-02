#include "Telegram.h"
#include "MessageDispatcher.h"
#include "Object.h"
#include "StateManager.h"
#include "ObjectContainer.h"

jeBegin

MessageDispatcher::Priority MessageDispatcher::messageQue_;

void MessageDispatcher::Discharge(Object* pReceiver, Telegram& msg)
{
	if (!pReceiver->HandleMessage(msg))
		jeDebugPrint("!MessageDispatcher - Message not handled!\n");
}

void MessageDispatcher::DispatchMessage(float delay, unsigned sender, unsigned receiver, const char* messageType, void *extraInfo)
{
	Object* pReceiver = OBJECT::pContainer_->GetObject(receiver);
	Telegram telegram(0.f, sender, receiver, messageType, extraInfo);

	if (delay <= 0.f)
		Discharge(pReceiver, telegram);

	else {
		float currentTime = StateManager::GetCurrentTime();
		telegram.dispatchTime = currentTime + delay;
		messageQue_.insert(telegram);
	}
}


void MessageDispatcher::DispatchDelayedMessage()
{
	float currentTime = StateManager::GetCurrentTime();

	// Check if either there is que to execute
	while (!messageQue_.empty() &&
		messageQue_.begin()->dispatchTime < currentTime)
	{
		// Get telegram
		Telegram telegram = *messageQue_.begin();
		// Get receiver
		Object* pReceiver = OBJECT::pContainer_->GetObject(telegram.receiverId);

		// Operate event and remove the message from the list
		Discharge(pReceiver, telegram);
		messageQue_.erase(messageQue_.begin());
	}
}

jeEnd
