#include <arduino.h>

#include "UltraSoundSensor.hpp"

UltraSoundSensor::UltraSoundSensor(pin_t triggerPin, pin_t echoPin)
    : _triggerPin{triggerPin}, _echoPin{echoPin} {}

UltraSoundSensor UltraSoundSensor::Create(pin_t triggerPin, pin_t echoPin)
{
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  return UltraSoundSensor(triggerPin, echoPin);
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