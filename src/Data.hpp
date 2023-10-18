#ifndef DATA_HPP
#define DATA_HPP
#include <stdint.h>

struct __attribute__((packed)) InterfaceData
{
    float gravity;
    float gravity_ratio;
    float orbit_rate;
    float orbit_ratio;
    float scope_width;
    float scope_phase;
    float ratio;
};

struct __attribute__((packed)) CvData
{
    float input_a;
    float input_b;
    float input_c;
    float input_d;
    float cv_gravity;
    float cv_orbit;
    float cv_scope;
    float cv_ratio;
};

struct __attribute__((packed)) ModeData
{
    char name[10]; // 9 characters + 1 for null terminator
    char type[10]; // 9 characters + 1 for null terminator
};

class Data
{

public:
    static uint8_t current_mode;
    static InterfaceData interface_data;
    static CvData cv_data;
    static float phase[4];
    static ModeData modes[4];
    static uint16_t output_value[4];

    static bool interfaceDataChanged;
    static bool cvDataChanged;
    static bool phaseDataChanged;
};

#endif // DATA_HPP
