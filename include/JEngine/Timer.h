#pragma once
#include "Macro.h"

jeBegin

struct Time{
	int year, month, day, hour, minute, second;
};

class Timer {

public:

	Timer():m_time(0.f) {};
	~Timer() {};

	void	Start(void);
	float	GetTime(void) const;

	static  Time GetCurrentTimeInfo(void);

private:

	float m_time;

	Timer(Timer&&) = delete;
	Timer(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;

};

jeEnd
