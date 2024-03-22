#include <arduino.h>

#include "BallSensor.hpp"
#include "Config.hpp"

BallSensor::BallSensor(float xPosition, pin_t triggerPin, pin_t echoPin)
    : _xPosition{xPosition}, _hasDetected{false}, _measuredDistance{0.0},
    _maxDistanceCM{Config::maxSensorDistanceCM}, _ultraSoundSensor{UltraSoundSensor::Create(triggerPin, echoPin)} {}

BallSensor BallSensor::Create(float xPosition, pin_t triggerPin, pin_t echoPin)
{
    return BallSensor(xPosition, triggerPin, echoPin);
}

float BallSensor::MeasureDistanceCM()
{
    float dist = _ultraSoundSensor.GetDistanceCM();

    if (dist != 0.0 && dist < _maxDistanceCM)
    {
        _hasDetected = true;
        _measuredDistance = dist;
    }

    return dist;
}

float BallSensor::CalibrateMaxDistance()
{
    float dist = _ultraSoundSensor.GetDistanceCM();

    _maxDistanceCM = dist - 1;

    return _maxDistanceCM;
}

void BallSensor::Reset()
{
    _hasDetected = false;
    _measuredDistance = 0.0;
}

BallSensor::operator bool()
{
    return _hasDetected;
}

bool BallSensor::HasDetected() const
{
    return _hasDetected;
}

float BallSensor::GetMeasuredDistanceCM() const
{
    return _measuredDistance;
}


float BallSensor::GetXPositionCM() const
{
    return _xPosition;
}

float BallSensor::GetMaxDistanceCM() const
{
    return _maxDistanceCM;
}