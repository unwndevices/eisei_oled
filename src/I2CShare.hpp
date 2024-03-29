#ifndef I2CSHARE_HPP
#define I2CSHARE_HPP
#include <Arduino.h>
#include <Wire.h>
#include "Data.hpp"

#define SLAVE_ADDRESS 0x11
#define MSG_READY 0x01

#define MSG_MODE_DATA 0x02
#define MSG_INTERFACE_DATA 0x03
#define MSG_CV_DATA 0x04
#define MSG_PHASE_DATA 0x05
#define MSG_OUTPUT_DATA 0x06
// Add more message IDs for future data...

class I2CShareSlave
{
public:
    I2CShareSlave()
    {
    }

    void Init()
    {
        Wire.begin(SLAVE_ADDRESS, 43, 44, 400000);
        Wire.onRequest(I2CShareSlave::transmitI2C);
        Wire.onReceive(I2CShareSlave::receiveI2C);
    }

    void Restart()
    {
        Wire.end();
        delay(100);
        Wire.begin(SLAVE_ADDRESS, 43, 44, 400000);
    }

    static void transmitI2C()
    {
        if (!ready)
        {
            Wire.write(MSG_READY);
            ready = true;
            return;
        }
        if (data.interfaceDataChanged)
        {
            Wire.write(MSG_INTERFACE_DATA);
            Wire.write((byte *)&data.interface_data, sizeof(data.interface_data));
            data.interfaceDataChanged = false;
        }
        else if (data.cvDataChanged)
        {
            Wire.write(MSG_CV_DATA);
            Wire.write((byte *)&data.cv_data, sizeof(data.cv_data));
            data.cvDataChanged = false;
        }
        else if (data.phaseDataChanged)
        {
            Wire.write(MSG_PHASE_DATA);
            Wire.write((byte *)&data.phase, sizeof(data.phase));
            data.phaseDataChanged = false;
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
            memcpy(&data.interface_data, buffer + 1, sizeof(data.interface_data));
            break;
        case MSG_CV_DATA:
            memcpy(&data.cv_data, buffer + 1, sizeof(data.cv_data));
            break;
        case MSG_PHASE_DATA:
            memcpy(&data.phase, buffer + 1, sizeof(data.phase));
            break;
        case MSG_MODE_DATA:
            memcpy(&data.modes, buffer + 1, sizeof(data.modes));
            break;
        case MSG_OUTPUT_DATA:
            memcpy(&data.output_value, buffer + 1, sizeof(data.output_value));
            break;
        // Add more cases for future data...
        default:
            // unknown message ID
            break;
        }
    }

    void updateInterfaceData(const InterfaceData &newData)
    {
        data.interface_data = newData;
        data.interfaceDataChanged = true;
    }

    void updateCvData(const CvData &newData)
    {
        data.cv_data = newData;
        data.cvDataChanged = true;
    }

    void updatePhaseData(const float newPhase[4])
    {
        memcpy(data.phase, newPhase, sizeof(data.phase));
        data.phaseDataChanged = true;
    }

private:
    static Data data;
    static bool ready;
};


#endif // I2CSHARE_HPP
