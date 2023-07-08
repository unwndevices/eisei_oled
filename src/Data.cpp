#include "Data.hpp"
bool Data::interfaceDataChanged = false;
bool Data::cvDataChanged = false;
bool Data::phaseDataChanged = false;

InterfaceData Data::interface_data = {
    50.0f,    // frequency
    0.1f,     // lfo_rate
    1.0f,     // lfo_multiplier
    0.0f,     // transimtter_rate
    0.5f,     // transimtter_width
    1.333333f // ratio
};

CvData Data::cv_data = {
    1.0f, // gravity_a
    1.0f, // gravity_b
    1.0f, // orbit_a
    1.0f, // orbit_b
    1.0f, // beam_a
    1.0f, // beam_b
    1.0f, // ratio_a
    1.0f  // ratio_b
};

float Data::phase[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.5f};