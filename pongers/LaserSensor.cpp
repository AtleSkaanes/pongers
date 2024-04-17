#include "LaserSensor.hpp"

#define START_ADDRESS 0x30

uint8_t LaserSensor::numSensors = 0;

LaserSensor::LaserSensor(VL53L0X sensor)
    : sensor{sensor} {}

LaserSensor LaserSensor::Create(pin_t shutDownPin, SensorMode mode = SensorMode::Default)
{
    digitalWrite(shutDownPin, HIGH);

    VL53L0X sensor = VL53L0X();

    sensor.setAddress(START_ADDRESS + numSensors);
    if (sensor.getAddress() != START_ADDRESS + numSensors)
    {
        Serial.print("Failed to set address to sensor with XSHUT pin ");
        Serial.println(shutDownPin);
    }

    if (!sensor.init())
    {
        Serial.println("Failed to start sensor with XSHUT pin ");
        Serial.println(shutDownPin);
    }
    numSensors++;

    if (mode == SensorMode::HighSpeed)
        sensor.setMeasurementTimingBudget(20000);

    else if (mode == SensorMode::HighAccuracy)
        sensor.setMeasurementTimingBudget(200000);

    return LaserSensor{sensor};
}

float LaserSensor::GetDistanceCM()
{
    return sensor.readRangeSingleMillimeters() / 10.0f;
}

uint8_t LaserSensor::GetLastStatusCode() const
{
    return sensor.last_status;
}