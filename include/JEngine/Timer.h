#pragma once
#include "Macro.h"

jeBegin

class Timer {

public:

	Timer():m_time(0.f) {};
	~Timer() {};

	void	Start(void);
	float	GetTime(void);

private:

	float m_time;

	Timer(Timer&&) = delete;
	Timer(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;

};

jeEnd
