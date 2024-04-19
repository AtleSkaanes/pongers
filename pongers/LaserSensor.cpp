#include "LaserSensor.hpp"

#define START_ADDRESS 0x30

uint8_t LaserSensor::numSensors = 0;

LaserSensor::LaserSensor(VL53L0X sensor)
    : sensor{sensor} {}

LaserSensor LaserSensor::Create(pin_t shutDownPin, SensorMode mode = SensorMode::Default)
{
    VL53L0X sensor;

    int newAddress = START_ADDRESS + numSensors;
    sensor.setAddress(newAddress);
    if (sensor.getAddress() != newAddress)
    {
        Serial.print("Failed to set address to sensor with XSHUT pin ");
        Serial.println(shutDownPin);
    }

    if (!sensor.init())
    {
        Serial.print("Failed to start sensor with XSHUT pin ");
        Serial.println(shutDownPin);
    }
    numSensors++;

    if (mode == SensorMode::HighSpeed)
        sensor.setMeasurementTimingBudget(20000);

    else if (mode == SensorMode::HighAccuracy)
        sensor.setMeasurementTimingBudget(200000);

    sensor.startContinuous();

    return LaserSensor{sensor};
}

LaserSensor *LaserSensor::CreateHeap(pin_t shutDownPin, SensorMode mode = SensorMode::Default)
{
    VL53L0X sensor;

    int newAddress = START_ADDRESS + numSensors;
    sensor.setAddress(newAddress);
    if (sensor.getAddress() != newAddress)
    {
        Serial.print("Failed to set address to sensor with XSHUT pin ");
        Serial.println(shutDownPin);
    }

    if (!sensor.init())
    {
        Serial.print("Failed to start sensor with XSHUT pin ");
        Serial.println(shutDownPin);
    }
    numSensors++;

    if (mode == SensorMode::HighSpeed)
        sensor.setMeasurementTimingBudget(20000);

    else if (mode == SensorMode::HighAccuracy)
        sensor.setMeasurementTimingBudget(200000);

    sensor.startContinuous();

    return new LaserSensor{sensor};
}

float LaserSensor::GetDistanceCM()
{

    uint16_t distMM = sensor.readRangeContinuousMillimeters();

    return static_cast<float>(distMM) / 10.0f - 0.75f;
}

uint8_t LaserSensor::GetLastStatusCode() const
{
    return sensor.last_status;
}