#pragma once

#include "UltraSoundSensor.hpp"
#include "typedefines.hpp"

class BallSensor {
private:
    float _xPosition;
    bool _hasDetected;
    float _measuredDistance;
    float _maxDistanceCM;
    UltraSoundSensor _ultraSoundSensor;

    BallSensor(float xPosition, pin_t triggerPin, pin_t echoPin);

public:
    BallSensor() = default;

    static BallSensor Create(float xPosition, pin_t triggerPin, pin_t echoPin);

    float MeasureDistanceCM();
    float CalibrateMaxDistance();

    void Reset();

    bool HasDetected() const;
    float GetMeasuredDistanceCM() const;
    float GetXPositionCM() const;
    float GetMaxDistanceCM() const;


    operator bool();
};