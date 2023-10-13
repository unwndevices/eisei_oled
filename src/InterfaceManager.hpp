#ifndef INTERFACEMANAGER_HPP
#define INTERFACEMANAGER_HPP
#include <unordered_map>

#include "Hardware.hpp"
#include "I2CShare.hpp"

class InterfaceManager
{
public:
    InterfaceManager(){};
    void Init()
    {
        i2c_slave.Init();
        hw.Init();
        i2c_slave.data.current_state.state = RunState::ESP_INITIALIZED;
        i2c_slave.data.stateDataChanged = true;
    }

    void Update()
    {
        hw.Update();
    };

    Button::State GetButtonState(int id)
    {
        return hw.GetButtonState(id);
    }

    Hardware hw;
    I2CShareSlave i2c_slave;
};

#endif // HARDWAREMANAGER_HPP
