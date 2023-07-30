#ifndef DATA_HPP
#define DATA_HPP

struct __attribute__((packed)) InterfaceData
{
    float gravity;
    float gravity_multiplier;
    float lfo_rate;
    float lfo_multiplier;
    float scope_attack;
    float scope_hold;
    float scope_decay;
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

class Data
{
public:
    static InterfaceData interface_data;
    static CvData cv_data;
    static float phase[5];

    static bool interfaceDataChanged;
    static bool cvDataChanged;
    static bool phaseDataChanged;
};

#endif// DATA_HPP
