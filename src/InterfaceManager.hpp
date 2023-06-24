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
        hw.Init();
        i2c_slave.Init();
    }

    void Update()
    {
        hw.Update();
    };

    Button::State GetButton(int id)
    {
        return hw.GetButton(id);
    }

    Hardware hw;
    I2CShareSlave i2c_slave;

};

#endif // HARDWAREMANAGER_HPP
