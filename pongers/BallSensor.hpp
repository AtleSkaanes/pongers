#pragma once

#include "UltraSoundSensor.hpp"

class BallSensor {
public:
  BallSensor(float xPosition, int triggerPin, int echoPin);

  float MeasureDistanceCM();
  float CalibrateMaxDistance();

  void Reset();

  bool HasDetected();
  float GetMeasuredDistanceCM();
  float GetXPositionCM();
  float GetMaxDistanceCM();


  operator bool();

private:
  UltraSoundSensor _ultraSoundSensor;
  float _xPosition;
  bool _hasDetected;
  float _measuredDistance;
  float _maxDistanceCM;
};