#pragma once
#include "Macro.h"

jeBegin

struct Time{
	int year, month, day, hour, minute, second;
};

class Timer {

public:

	Timer():time_(0.f) {};
	~Timer() {};

	void	Start(void);
	float	GetTime(void) const;

	static  Time GetCurrentTimeInfo(void);

private:

	float time_;

	Timer(Timer&&) = delete;
	Timer(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;

};

jeEnd
