#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\LaserSensor.cpp"
#include "LaserSensor.hpp"

uint8_t LaserSensor::numSensors = 0;

LaserSensor::LaserSensor(Adafruit_VL53L0X sensor)
    : sensor{sensor} {}

LaserSensor LaserSensor::Create(pin_t shutDownPin)
{
    digitalWrite(shutDownPin, HIGH);

    Adafruit_VL53L0X sensor;
    if (!sensor.begin(0x30 + numSensors))
    {
        Serial.print(shutDownPin);
        Serial.println(": ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    sensor.setDeviceMode(Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_ACCURACY);
    numSensors++;

    return LaserSensor{sensor};
}

float LaserSensor::GetDistanceCM()
{
    static VL53L0X_RangingMeasurementData_t measure;

    sensor.rangingTest(&measure);

    if (measure.RangeStatus == 4)
    {
        Serial.print("Measure Error! ");
        return 0.0f;
    }

    return measure.RangeMilliMeter / 10.0f;
}

uint8_t LaserSensor::GetLast