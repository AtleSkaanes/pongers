#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\BallSensor.cpp"
#include <arduino.h>

#include "BallSensor.hpp"
#include "Config.hpp"

BallSensor::BallSensor(float xPosition, LaserSensor sensor)
    : _xPosition{xPosition}, _hasDetected{false}, _measuredDistance{0.0},
      _maxDistanceCM{Config::maxSensorDistanceCM}, _sensor{sensor} {}

BallSensor BallSensor::Create(float xPosition, LaserSensor sensor)
{
    return BallSensor(xPosition, sensor);
}

float BallSensor::MeasureDistanceCM()
{
    float dist = _sensor.GetDistanceCM();

    if (dist != 0.0 && dist < _maxDistanceCM)
    {
        _hasDetected = true;
        _measuredDistance = dist;
    }

    return dist;
}

float BallSensor::CalibrateMaxDistance()
{
    float dist = _sensor.GetDistanceCM();

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