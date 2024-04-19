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

BallSensor *BallSensor::CreateHeap(float xPosition, LaserSensor sensor)
{
    return new BallSensor(xPosition, sensor);
}

float BallSensor::MeasureDistanceCM()
{
    float dist = _sensor.GetDistanceCM();

    if (dist != 0.0f && dist < _maxDistanceCM && dist == dist)
    {
        _hasDetected = true;
        _measuredDistance = dist;
    }

    return dist;
}

float BallSensor::CalibrateMaxDistance()
{
    float dist = _sensor.GetDistanceCM();
    Serial.print("Calibrated max dist to:\t");
    Serial.println(dist);

    _maxDistanceCM = dist - 2.5;

    return _maxDistanceCM;
}

void BallSensor::Reset()
{
    _hasDetected = false;
    _measuredDistance = 0.0f;
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

void BallSensor::SetMeasuredDistanceCM(float distanceCM)
{
    _hasDetected = true;
    _measuredDistance = distanceCM;
}

float BallSensor::GetXPositionCM() const
{
    return _xPosition;
}

float BallSensor::GetMaxDistanceCM() const
{
    return _maxDistanceCM;
}

void BallSensor::SetMaxDistanceCM(float distanceCM)
{
    _maxDistanceCM = distanceCM;
}

uint8_t BallSensor::GetLastStatus() const
{
    return _sensor.GetLastStatusCode();
}