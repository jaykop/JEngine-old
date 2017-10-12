#pragma once
#include "Macro.h"
#include <ctime>

NS_JE_BEGIN

class Timer {

public:

	Timer():m_time(0.f) {};
	~Timer() {};

	void	Start(void);
	float	GetTime(void);

private:

	Timer(const Timer& /*_cpoy*/) {};
	void operator=(const Timer& /*_cpoy*/) {};

	float m_time;

};

NS_JE_END