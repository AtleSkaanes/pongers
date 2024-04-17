#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\UltraSoundSensor.hpp"
#pragma once

#include "typedefines.hpp"

class UltraSoundSensor
{
private:
    pin_t _triggerPin;
    pin_t _echoPin;
    UltraSoundSensor(pin_t triggerPin, pin_t echoPin);

public:
    UltraSoundSensor() = default;

    static UltraSoundSensor Create(pin_t triggerPin, pin_t echoPin);

    float GetDistanceCM() const;
};