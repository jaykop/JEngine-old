#include <ctime>
#include "Timer.h"

jeBegin

/******************************************************************************/
/*!
\brief - Start to check time
*/
/******************************************************************************/
void Timer::Start(void)
{
	time_ = static_cast<float>(clock());
}

/******************************************************************************/
/*!
\brief - Get spent time from StartTime() moment
*/
/******************************************************************************/
float Timer::GetTime(void) const
{
	return (static_cast<float>(clock()) - time_) / static_cast<float>(CLOCKS_PER_SEC);
}

/******************************************************************************/
/*!
\brief - Get current time info (yy/mm/dd/hh/mm/ss)
\return - Time instance
*/
/******************************************************************************/
Time Timer::GetCurrentTimeInfo()
{
	time_t theTime = time(nullptr);
	tm timeBuf;
	localtime_s(&timeBuf, &theTime);

	return Time{ timeBuf.tm_year + 1900, 
		timeBuf.tm_mon + 1, 
		timeBuf.tm_mday, 
		timeBuf.tm_hour, 
		timeBuf.tm_min, 
		timeBuf.tm_sec };
}

jeEnd

