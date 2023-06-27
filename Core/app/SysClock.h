/*
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#ifndef APP_SYSCLOCK_H_
#define APP_SYSCLOCK_H_

#include <chrono>

class SysClockSecondsSinceStart
{
public:
    // 500MHz, or whatever you need
    using period                    = std::ratio<1, 1>;
    using rep                       = uint32_t;
    using duration                  = std::chrono::duration<rep, period>;
    using time_point                = std::chrono::time_point<SysClockSecondsSinceStart>;
    static constexpr bool is_steady = false;

    static time_point now() noexcept;
};

#endif /* APP_SYSCLOCK_H_ */
