#ifndef I2CSHARE_HPP
#define I2CSHARE_HPP
#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x11

#define MSG_INTERFACE_DATA 0x01
#define MSG_CV_DATA 0x02
#define MSG_PHASE_DATA 0x03
// Add more message IDs for future data...

struct __attribute__((packed)) InterfaceData
{
    float gravity;
    float lfo_rate;
    float lfo_multiplier;
    float transmitter_width;
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

class I2CShareSlave
{
public:
    static InterfaceData interface_data;
    static CvData cv_data;
    static float phase[5];

    I2CShareSlave()
    {
    }

    void Init()
    {
        Wire.begin(SLAVE_ADDRESS, 43, 44, 400000);
        Wire.onRequest(I2CShareSlave::transmitI2C);
        Wire.onReceive(I2CShareSlave::receiveI2C);
    }

    static void transmitI2C()
    {

        if (interfaceDataChanged)
        {
            Wire.write(MSG_INTERFACE_DATA);
            Wire.write((byte *)&interface_data, sizeof(interface_data));
            interfaceDataChanged = false;
        }
        else if (cvDataChanged)
        {
            Wire.write(MSG_CV_DATA);
            Wire.write((byte *)&cv_data, sizeof(cv_data));
            cvDataChanged = false;
        }
        else if (phaseDataChanged)
        {
            Wire.write(MSG_PHASE_DATA);
            Wire.write((byte *)&phase, sizeof(phase));
            phaseDataChanged = false;
        }
        // Add more else if blocks for future data...
    }

    static void receiveI2C(int byteCount)
    {
        byte buffer[byteCount];
        int i = 0;

        while (Wire.available())
        {
            buffer[i++] = Wire.read();
        }

        // process the received data based on its message ID
        switch (buffer[0])
        {
        case MSG_INTERFACE_DATA:
            memcpy(&interface_data, buffer + 1, sizeof(interface_data));
            break;
        case MSG_CV_DATA:
            memcpy(&cv_data, buffer + 1, sizeof(cv_data));
            break;
        case MSG_PHASE_DATA:
            memcpy(&phase, buffer + 1, sizeof(phase));
            break;
        // Add more cases for future data...
        default:
            // unknown message ID
            break;
        }
    }

    void updateInterfaceData(const InterfaceData &newData)
    {
        interface_data = newData;
        interfaceDataChanged = true;
    }

    void updateCvData(const CvData &newData)
    {
        cv_data = newData;
        cvDataChanged = true;
    }

    void updatePhaseData(const float newPhase[5])
    {
        memcpy(phase, newPhase, sizeof(phase));
        phaseDataChanged = true;
    }

    // Add more update functions for future data...

private:
    static bool interfaceDataChanged;
    static bool cvDataChanged;
    static bool phaseDataChanged;
    // Add more change flags for future data...
};
// Usage:
// I2CShareSlave i2cDevice;
// ...
// i2cDevice.updateInterfaceData(newInterfaceData);
// i2cDevice.updateCvData(newCvData);
// i2cDevice.updatePhaseData(newPhase);

#endif // I2CSHARE_HPP
