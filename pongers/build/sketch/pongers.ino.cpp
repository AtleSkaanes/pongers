#include <Arduino.h>
#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino"
#include "Config.hpp"
// #include "BallSensor.hpp"
#include "LaserSensor.hpp"
#include <VL53L0X.h>

// BallSensor sensor1 = BallSensor::Create(34.5, 8, 9);
// BallSensor sensor2 = BallSensor::Create(26.5, 3, 4);

LaserSensor *sensors[2];

#line 11 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino"
void setup();
#line 38 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino"
void loop();
#line 11 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino"
void setup()
{
    Serial.begin(9600);
    Serial.println("\nSTARTING UP!");

    // while (sensor1.GetMaxDistanceCM() == -1 || sensor1.GetMaxDistanceCM() == Config::maxSensorDistanceCM)
    // {
    //     sensor1.CalibrateMaxDistance();
    // }
    // while (sensor2.GetMaxDistanceCM() == -1 || sensor2.GetMaxDistanceCM() == Config::maxSensorDistanceCM)
    // {
    //     sensor2.CalibrateMaxDistance();
    // }

    // Serial.println("MAX DISTANCE:");
    // Serial.print("SENSOR 1: ");
    // Serial.println(sensor1.GetMaxDistanceCM());

    // Serial.print("SENSOR 2: ");
    // Serial.println(sensor2.GetMaxDistanceCM());
    // Serial.println("\n");

    pin_t shutDownPins[2] = {8, 9};
    CreateSensors<2>(shutDownPins, sensors);
    Serial.println("READY");
}

void loop()
{
    Serial.print("Dist 1:\t");

    float dist1 = sensors[0]->GetDistanceCM();

    Serial.print(dist1);
    Serial.print(" cm\t\tDist 2:\t");

    float dist2 = sensors[1]->GetDistanceCM();

    Serial.print(dist2);
    Serial.println(" cm");

    return;

    // if (sensor1.HasDetected() && sensor2.HasDetected())
    //     return;

    // // Serial.print("FIRST: ");
    // // Serial.print(sensor1.MeasureDistanceCM());
    // // Serial.print("\tSECOND: ");
    // // Serial.println(sensor2.MeasureDistanceCM());

    // if (!sensor1.HasDetected())
    // {
    //     sensor1.MeasureDistanceCM();
    //     if (!sensor1.HasDetected())
    //         return;

    //     Serial.print("FIRST DETECTED: ");
    //     Serial.println(sensor1.GetMeasuredDistanceCM());
    // }

    // if (!sensor2.HasDetected())
    // {
    //     sensor2.MeasureDistanceCM();
    //     if (!sensor2.HasDetected())
    //         return;

    //     Serial.print("SECOND DETECTED: ");
    //     Serial.println(sensor2.GetMeasuredDistanceCM());
    // }

    // Serial.println("\nBALL MEASUREMENTS:");
    // Serial.print("FIRST: ");
    // Serial.println(sensor1.GetMeasuredDistanceCM());
    // Serial.print("SECOND: ");
    // Serial.println(sensor2.GetMeasuredDistanceCM());

    // float endPoint = PredictEndPointCM(sensor1, sensor2);
    // Serial.print("ENDPOINT: ");
    // Serial.println(endPoint);
}

// float PredictEndPointCM(BallSensor sensor1, BallSensor sensor2)
// {
//     const float deltaHeigt = sensor2.GetMeasuredDistanceCM() - sensor1.GetMeasuredDistanceCM();
//     const float deltaWidth = abs(sensor1.GetXPositionCM() - sensor2.GetXPositionCM());

//     // How many cm the ball goes up, for each cm it goes forwards
//     const float slope = deltaHeigt / deltaWidth;

//     // What y-coordinate (in cm), the ball SHOULD have, bouncing not included.
//     const float rawYvalue = slope * sensor2.GetXPositionCM() + sensor2.GetMeasuredDistanceCM();

//     // This value indicates if the balls slope is positve (+1), or negtaive (-1)
//     const int sign = slope == 0 ? 1 : slope / abs(slope);

//     // This value indicates how many "surfaces" the ball is in (+1 for each bounce)
//     int times = 1;

//     // Change the balls "anchor point", to the lower or higher left corner.
//     float currentBallY = rawYvalue + (Config::ballDiameterCM / 2 * sign);
//     // If The ball went in a straight line, and the field looped, what would the height of the fields be?
//     int currentHeight = Config::frameHeightCM;

//     // Calculate the correct value for "Times"
//     while (currentBallY > currentHeight || currentBallY < -currentHeight + Config::frameHeightCM)
//     {
//         currentBallY = rawYvalue + (Config::ballDiameterCM / 2 * (times + 1) * sign);
//         currentHeight = Config::frameHeightCM * (times + 1);
//         times++;
//     }
//     times--;

//     // Get the new y-value.
//     const float modifiedYvalue = rawYvalue + Config::ballDiameterCM * times * sign;

//     // Calculate where the ball ends, when bouncing is included.
//     float endValue;
//     if (times % 2 == 0)
//     {
//         endValue = fmod(fmod(modifiedYvalue, Config::frameHeightCM) + Config::frameHeightCM, Config::frameHeightCM);
//     }
//     else
//     {
//         endValue = Config::frameHeightCM - fmod((fmod(modifiedYvalue, Config::frameHeightCM) + Config::frameHeightCM), Config::frameHeightCM);
//     }

//     return endValue;
// }

