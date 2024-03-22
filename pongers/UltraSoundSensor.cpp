#include <arduino.h>

#include "UltraSoundSensor.hpp"

UltraSoundSensor::UltraSoundSensor(int triggerPin, int echoPin)
  : _triggerPin{triggerPin}, _echoPin{echoPin}
{
  pinMode(_triggerPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

float UltraSoundSensor::GetDistanceCM() const
{
  // Send a trigger signal that is 10 uS long
  digitalWrite(_triggerPin, HIGH);
  delayMicroseconds(10); // Delay my 10 microseconds
  digitalWrite(_triggerPin, LOW);
  
  // Read echo pin
  unsigned long response_us = pulseIn(_echoPin, HIGH, 100000); // Waits 100000 microseconds for echo pin to turn HIGH.

  float distanceCM = response_us / 58.0; // Response time in uS divided by weird constant, to get the measurement in CM.


  return distanceCM;
}