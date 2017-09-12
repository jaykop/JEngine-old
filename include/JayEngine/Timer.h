#pragma once
#include <ctime>
#include "Macro.h"

NS_JE_BEGIN

class Timer {

public:

	static void		Start(void);
	static float	GetTime(void);

private:

	static float m_time;

};

NS_JE_END