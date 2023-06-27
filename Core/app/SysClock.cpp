/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "SysClock.h"
#ifdef WIN32
# include <windows.h>
#endif
#ifdef STM32F401xE
# include "stm32f4xx_hal.h"
#endif

using namespace std::chrono;

SysClockSecondsSinceStart::time_point SysClockSecondsSinceStart::now() noexcept
{
#ifdef WIN32
	return time_point{duration{GetTickCount()/1000}};
#endif
#ifdef STM32F401xE
	return time_point{duration{HAL_GetTick()/1000}};
#endif
}


