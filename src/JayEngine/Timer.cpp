#include <ctime>
#include "Timer.h"

NS_JE_BEGIN

/******************************************************************************/
/*!
\brief - Start to check time
*/
/******************************************************************************/
void Timer::Start(void)
{
	m_time = static_cast<float>(clock());
}

/******************************************************************************/
/*!
\brief - Get spent time from StartTime() moment
*/
/******************************************************************************/
float Timer::GetTime(void)
{
	return (static_cast<float>(clock()) - m_time) / (CLOCKS_PER_SEC);
}

NS_JE_END

