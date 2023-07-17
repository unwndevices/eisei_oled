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
    float cv_gravity_a;
    float cv_gravity_b;
    float cv_orbit_a;
    float cv_orbit_b;
    float cv_beam_a;
    float cv_beam_b;
    float cv_ratio_a;
    float cv_ratio_b;
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

#endif // DATA_HPP
