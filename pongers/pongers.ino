#include "Config.hpp"
#include "BallSensor.hpp"

BallSensor sensor1 = BallSensor(34.5, 3, 4);
BallSensor sensor2 = BallSensor(26, 8, 9);

void setup() {
    Serial.begin(9600);
    Serial.println("\nSTARTING UP!");

    sensor1.CalibrateMaxDistance();
    sensor2.CalibrateMaxDistance();

    Serial.println("MAX DISTANCE:");
    Serial.print("SENSOR 1: ");
    Serial.println(sensor1.GetMaxDistanceCM());

    Serial.print("SENSOR 2: ");
    Serial.println(sensor2.GetMaxDistanceCM());
    Serial.println("\n");
}

void loop() {

    if (sensor1.HasDetected() && sensor2.HasDetected())
        return;

    // Serial.print("FIRST: ");
    // Serial.print(sensor1.MeasureDistanceCM());
    // Serial.print("\tSECOND: ");
    // Serial.println(sensor2.MeasureDistanceCM());

    if (!sensor1.HasDetected()) {
        sensor1.MeasureDistanceCM();
    }
    if (!sensor2.HasDetected()) {
        sensor2.MeasureDistanceCM();
    }

    if (!sensor1.HasDetected()) {
        sensor2.Reset();
        return;
    }

    // if (!sensor1.HasDetected()) {
    //     sensor1.MeasureDistanceCM();
    //     if (!sensor1.HasDetected())
    //         return;

    //     Serial.print("FIRST DETECTED: ");
    //     Serial.println(sensor1.GetMeasuredDistanceCM());
    // }

    // if (!sensor2.HasDetected()) {
    //     sensor2.MeasureDistanceCM();
    //     if (!sensor2.HasDetected())
    //         return;

    //     Serial.print("SECOND DETECTED: ");
    //     Serial.println(sensor2.GetMeasuredDistanceCM());
    // }



    Serial.println("BALL MEASUREMENTS:");
    Serial.print("FIRST: ");
    Serial.println(sensor1.GetMeasuredDistanceCM());
    Serial.print("SECOND: ");
    Serial.println(sensor2.GetMeasuredDistanceCM());

    float endPoint = PredictEndPointCM(sensor1, sensor2);
    Serial.print("ENDPOINT: ");
    Serial.println(endPoint);
}

float PredictEndPointCM(BallSensor sensor1, BallSensor sensor2) {
    const float deltaHeigt = sensor2.GetMeasuredDistanceCM() - sensor1.GetMeasuredDistanceCM();
    const float deltaWidth = abs(sensor1.GetXPositionCM() - sensor2.GetXPositionCM());

    // How many cm the ball goes up, for each cm it goes forwards
    const float slope = deltaHeigt / deltaWidth;

    // What y-coordinate (in cm), the ball SHOULD have, bouncing not included.
    const float rawYvalue = slope * sensor2.GetXPositionCM() + sensor2.GetMeasuredDistanceCM();

    // This value indicates if the balls slope is positve (+1), or negtaive (-1)
    const int sign = slope == 0 ? 1 : slope / abs(slope);

    // This value indicates how many "surfaces" the ball is in (+1 for each bounce)
    int times = 1;

    // Change the balls "anchor point", to the lower or higher left corner.
    float currentBallY = rawYvalue + (Config::ballDiameterCM / 2 * sign);
    // If The ball went in a straight line, and the field looped, what would the height of the fields be?
    int currentHeight = Config::frameHeightCM;

    // Calculate the correct value for "Times"
    while (currentBallY > currentHeight || currentBallY < -currentHeight + Config::frameHeightCM) {
        currentBallY = rawYvalue + (Config::ballDiameterCM / 2 * (times + 1) * sign);
        currentHeight = Config::frameHeightCM * (times + 1);
        times++;
    }
    times--;
    
    // Get the new y-value.
    const float modifiedYvalue = rawYvalue + Config::ballDiameterCM * times * sign;

    // Calculate where the ball ends, when bouncing is included.
    float endValue;
    if (times % 2 == 0) {
        endValue = fmod(fmod(modifiedYvalue, Config::frameHeightCM) + Config::frameHeightCM, Config::frameHeightCM);
    } else {
        endValue = Config::frameHeightCM - fmod((fmod(modifiedYvalue, Config::frameHeightCM) + Config::frameHeightCM), Config::frameHeightCM);
    }

    return endValue;
}
