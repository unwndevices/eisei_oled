#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP
#include <stdint.h>

struct __attribute__((packed)) SharedData
{
    float frequency;
    float lfo_rate;
    float transmitter_phase;
    float transmitter_width;
    float ratio;
};

extern SharedData system_data;
extern float phase[4];

#endif // !SHAREDDATA_HPP
