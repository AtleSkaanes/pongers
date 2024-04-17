#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\LaserSensor.hpp"
#pragma once

#include <VL53L0X.h>
#include "typedefines.hpp"

enum SensorMode
{
    Default,
    HighSpeed,
    HighAccuracy
};

class LaserSensor
{
private:
    VL53L0X sensor;
    static uint8_t numSensors;

    LaserSensor(VL53L0X sensor);

public:
    static LaserSensor Create(pin_t shutDownPin, SensorMode mode = SensorMode::Default);

    float GetDistanceCM();

    uint8_t GetLastStatusCode() const;
};

template <uint8_t size>
void CreateSensors(pin_t shutDownPins[size], LaserSensor *out[size], SensorMode mode = SensorMode::Default)
{
    for (uint8_t i = 0; i < size; i++)
    {
        pinMode(shutDownPins[i], OUTPUT);
        digitalWrite(shutDownPins[i], LOW);
    }

    delay(10);

    for (uint8_t i = 0; i < size; i++)
    {
        digitalWrite(shutDownPins[i], HIGH);
    }

    delay(10);

    for (uint8_t i = 0; i < size; i++)
    {
        digitalWrite(shutDownPins[i], LOW);
    }

    delay(10);

    for (uint8_t i = 0; i < size; i++)
    {
        out[i] = &LaserSensor::Create(shutDownPins[i], mode);
    }
}
