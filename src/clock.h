// Code obtenu sur StackOverflow : https://stackoverflow.com/a/10466612

#pragma once

#if !defined(_WIN32) && !defined(_WIN64) // Linux - Unix
    #  include <sys/time.h>
    typedef timeval sys_time_t;
    inline void system_time(sys_time_t* t) {
        gettimeofday(t, nullptr);
    }
    inline long long time_to_msec(const sys_time_t& t) {
        return t.tv_sec * 1000LL + t.tv_usec / 1000;
    }
    #else // Windows and MinGW
    #  include <sys/timeb.h>
    typedef _timeb sys_time_t;
    inline void system_time(sys_time_t* t) { _ftime(t); }
    inline long long time_to_msec(const sys_time_t& t) {
        return t.time * 1000LL + t.millitm;
    }
#endif

inline long long getTimeInMs()
{
    sys_time_t t;
    system_time(&t);

    long long currentTimeMs = time_to_msec(t);
    return currentTimeMs;
}