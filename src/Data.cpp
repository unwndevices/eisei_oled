#include "Data.hpp"
bool Data::interfaceDataChanged = false;
bool Data::cvDataChanged = false;
bool Data::phaseDataChanged = false;
bool Data::stateDataChanged = false;

InterfaceData Data::interface_data = {
    50.0f, // frequency
    1.0f,  // gravity_ratio
    0.02f, // lfo_rate
    1.0f,  // lfo_multiplier
    0.0f,  // scope_width
    0.5f,  // scope_phase
    1.0f   // ratio
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

ModeData Data::modes[4] = {{"", ""}, {"", ""}, {"", ""}, {"", ""}};

float Data::phase[4] = {0.0f, 0.0f, 0.0f, 0.0f};

StateData Data::current_state = {RunState::STOPPED, 0};
uint16_t Data::output_value[4] = {0, 0, 0, 0};