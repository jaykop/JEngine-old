#pragma once
#include <iostream>
#include "Macro.h"

JE_BEGIN;

struct Telegram
{
	unsigned senderId = 0, receiverId = 0;
	const char* message = "Message_None";
	float dispatchTime = 0.f;
	void *extraInfo = nullptr;

	Telegram(float _dispatchTime, unsigned _senderId, unsigned _receiverId, 
		const char* _msg, void* _extraInfo)
		: dispatchTime(_dispatchTime), senderId(_senderId), receiverId(_receiverId),
	message(_msg), extraInfo(_extraInfo) {}
};

static const float SmallestDelay = 0.25f;

inline bool operator==(const Telegram& left, const Telegram& right)
{
	return (fabs(left.dispatchTime - right.dispatchTime) < SmallestDelay
		&& left.senderId == right.senderId
		&& left.receiverId == right.receiverId
		&& !strcmp(left.message, right.message));
}

inline bool operator<(const Telegram& left, const Telegram& right)
{
	// Return false when two telegrams are same
	if (left == right)
		return false;
	
	return (left.dispatchTime < right.dispatchTime);
}

inline std::ostream& operator<< (std::ostream& os, const Telegram& msg)
{
	os << "*Telegram - Time: " << msg.dispatchTime << " Sender: " << msg.senderId
		<< " Receiver: " << msg.receiverId << " Message: " << msg.message << "\n";

	return os;
}

inline void PrintMessageInfo(const Telegram& msg)
{
	JE_DEBUG_PRINT("*Telegram - Time: %f / Sender: %i / Receiver: %i / Message: %s\n", 
		msg.dispatchTime, msg.senderId, msg.receiverId, msg.message);
}

template <class T>
inline T DereferenceToType(void *p)
{
	// A C-style cast of the form (T) is defined as trying to do a static_cast if possible, 
	// falling back on a reinterpret_cast if that doesn't work. 
	// It also will apply a const_cast if it absolutely must.
	return *(T*)(p);
}

JE_END;