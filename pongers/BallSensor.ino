#include "BallSensor.hpp"
#include "Config.hpp"

BallSensor::BallSensor(float xPosition, int triggerPin, int echoPin)
{
  _xPosition = xPosition;
  _hasDetected = false;
  _measuredDistance = 0.0;
  _maxDistanceCM = Config::maxSensorDistanceCM;
  _ultraSoundSensor = UltraSoundSensor(triggerPin, echoPin);
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

bool BallSensor::HasDetected()
{
  return _hasDetected;
}

float BallSensor::GetMeasuredDistanceCM()
{
  return _measuredDistance;
}


float BallSensor::GetXPositionCM()
{
  return _xPosition;
}

float BallSensor::GetMaxDistanceCM()
{
    return _maxDistanceCM;
}