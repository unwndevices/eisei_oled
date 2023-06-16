#ifndef TIMER_HPP
#define TIMER_HPP
#include <Arduino.h>

class Timer
{
public:
    Timer() : start_time(0),
              current_time(0),
              delta_time(0),
              is_started(false){};

    void CalculateDeltaTime();
    uint16_t GetDeltaTime() { return delta_time; };
    ulong GetCurrentTime() { return current_time; };

    void Restart();

private:
    // time elapsed from last call, in ms
    uint16_t delta_time;
    ulong start_time;
    ulong current_time;
    
    bool is_started;
};

void Timer::Restart()
{
    is_started = true;
    start_time = millis();
}

void Timer::CalculateDeltaTime()
{
    delta_time = millis() - (current_time + start_time);
    current_time = millis() - start_time;
}

#endif