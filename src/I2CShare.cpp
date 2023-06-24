#include "I2CShare.hpp"
bool I2CShareSlave::interfaceDataChanged = false;
bool I2CShareSlave::cvDataChanged = false;
bool I2CShareSlave::phaseDataChanged = false;

InterfaceData I2CShareSlave::interface_data = {
    50.0f,    // frequency
    0.1f,     // lfo_rate
    1.0f,     // lfo_multiplier
    0.0f,     // transimtter_width
    1.333333f // ratio
};

CvData I2CShareSlave::cv_data = {
    1.0f, // gravity_a
    1.0f, // gravity_b
    1.0f, // orbit_a
    1.0f, // orbit_b
    1.0f, // beam_a
    1.0f, // beam_b
    1.0f, // ratio_a
    1.0f  // ratio_b
};

float I2CShareSlave::phase[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.5f};