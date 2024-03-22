#pragma once

class UltraSoundSensor
{
public:
  UltraSoundSensor(int triggerPin, int echoPin);
  UltraSoundSensor() = default;

  float GetDistanceCM() const;
  float GetMaxDistanceCM();

private:
  int _triggerPin;
  int _echoPin;
};