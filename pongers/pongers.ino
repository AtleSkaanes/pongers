#include "Config.hpp"
#include "BallSensor.hpp"
#include "LaserSensor.hpp"
#include "AIStepper.hpp"

BallSensor *sensor1 = nullptr;
BallSensor *sensor2 = nullptr;

AIStepper stepper;

bool ballIsReturning = false;

constexpr pin_t STEP_PIN = 3;
constexpr pin_t DIR_PIN = 2;
constexpr pin_t BUTTON_PIN = 5;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Serial.println(F("\nSTARTING UP!"));

    LaserSensor *sensors[2] = {nullptr, nullptr};
    pin_t shutDownPins[2] = {8, 9};

    CreateSensors<2>(shutDownPins, sensors, SensorMode::HighSpeed);

    sensor1 = BallSensor::CreateHeap(34.5, *sensors[0]);
    sensor2 = BallSensor::CreateHeap(25.5, *sensors[1]);

    Serial.println(F("[STATUS]: CALIBRATING SENSORS"));

    // Wait until the max distance is a valid number

    while (sensor1->GetMaxDistanceCM() <= 1.0f || sensor1->GetMaxDistanceCM() == Config::maxSensorDistanceCM || sensor1->GetMaxDistanceCM() != sensor1->GetMaxDistanceCM())
    {
        sensor1->CalibrateMaxDistance();
    }
    while (sensor2->GetMaxDistanceCM() <= 1.0f || sensor2->GetMaxDistanceCM() == Config::maxSensorDistanceCM || sensor1->GetMaxDistanceCM() != sensor1->GetMaxDistanceCM())
    {
        sensor2->CalibrateMaxDistance();
    }

    Serial.print(F("[READING]: Max dist 1:\t"));
    Serial.print(sensor1->GetMaxDistanceCM());

    Serial.print(F(" cm\t\tMax dist 2:\t"));
    Serial.println(sensor2->GetMaxDistanceCM());
    Serial.println(F(" cm\n"));

    float minDist = (sensor1->GetMaxDistanceCM() < sensor2->GetMaxDistanceCM()) ? sensor1->GetMaxDistanceCM() : sensor2->GetMaxDistanceCM();
    sensor1->SetMaxDistanceCM(minDist);
    sensor2->SetMaxDistanceCM(minDist);

    Config::frameHeightCM = minDist;

    Serial.println(F("\n[STATUS]: CALIBRATING STEPPER MOTOR"));

    stepper = AIStepper::Create(STEP_PIN, DIR_PIN, 200);

    stepper.Calibrate(BUTTON_PIN, BUTTON_PIN);

    Serial.println(F("\n[STATUS]: READY"));
}

void loop()
{
    stepper.UpdatePos();
    // float dist1 = sensor1->MeasureDistanceCM();
    // if (dist1 >= sensor1->GetMaxDistanceCM())
    //     return;
    // Serial.print("Sensor 1:\t");
    // Serial.print(dist1);
    // Serial.println(" cm");

    // return;

    // float dist2 = sensor2->MeasureDistanceCM();

    // if (dist1 < sensor1->GetMaxDistanceCM())
    // {
    //     Serial.print("Sensor 1:\t");
    //     Serial.print(dist1);
    //     Serial.println(" cm");
    // }

    // if (dist2 < sensor2->GetMaxDistanceCM())
    // {
    //     Serial.print("Sensor 2:\t");
    //     Serial.print(dist2);
    //     Serial.println(" cm");
    // }

    // return;

    if (sensor1->HasDetected() && sensor2->HasDetected())
    {
        return;
    }

    if (sensor2->HasDetected() && !sensor1->HasDetected())
    {
        Serial.println(F("[GAME STATUS]: ball returning"));
        ballIsReturning = true;
    }

    if (sensor1->HasDetected() && ballIsReturning)
    {
        sensor1->Reset();
        sensor2->Reset();
        Serial.println(F("[GAME STATUS]: Ball returned; Resat sensors"));
        ballIsReturning = false;
    }

    // Serial.print("FIRST: ");
    // Serial.print(sensor1.MeasureDistanceCM());
    // Serial.print("\tSECOND: ");
    // Serial.println(sensor2.MeasureDistanceCM());

    if (!sensor1->HasDetected())
    {
        float minDist = sensor1->GetMaxDistanceCM();

        do
        {
            sensor1->MeasureDistanceCM();
            if (sensor1->HasDetected() && sensor1->GetMeasuredDistanceCM() < minDist)
            {
                minDist = sensor1->GetMeasuredDistanceCM();
            }

        } while (sensor1->HasDetected() && sensor1->GetMeasuredDistanceCM() < minDist + 0.5f);

        if (!sensor1->HasDetected())
            return;

        sensor1->SetMeasuredDistanceCM(minDist);

        Serial.print(F("[READING]: FIRST SENSOR DETECTED:\t"));
        Serial.print(sensor1->GetMeasuredDistanceCM());
        Serial.println(F(" cm"));
    }

    if (!sensor2->HasDetected())
    {
        float minDist = sensor2->GetMaxDistanceCM();

        do
        {
            sensor2->MeasureDistanceCM();
            if (sensor2->HasDetected() && sensor2->GetMeasuredDistanceCM() < minDist)
            {
                minDist = sensor2->GetMeasuredDistanceCM();
            }

        } while (sensor2->HasDetected() && sensor2->GetMeasuredDistanceCM() < minDist + 0.5f);

        if (!sensor2->HasDetected())
            return;

        sensor2->SetMeasuredDistanceCM(minDist);

        Serial.print(F("[READING]: SECOND SENSOR DETECTED:\t"));
        Serial.print(sensor2->GetMeasuredDistanceCM());
        Serial.println(F(" cm"));
    }

    Serial.println(F("\n[GAME STATUS]: ALL SENSORS DETECTED BALL"));
    Serial.println(F("\nBALL MEASUREMENTS:"));
    Serial.print(F("FIRST: "));
    Serial.println(sensor1->GetMeasuredDistanceCM());
    Serial.print(F("SECOND: "));
    Serial.println(sensor2->GetMeasuredDistanceCM());

    float endPoint = PredictEndPointCM(*sensor1, *sensor2);
    Serial.print(F("\nENDPOINT: "));
    Serial.println(endPoint);

    stepper.Move(endPoint, 10);

    // delay(200);

    // sensor2->Reset();
    // sensor1->Reset();
}

float PredictEndPointCM(BallSensor sensor1, BallSensor sensor2)
{
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
    while (currentBallY > currentHeight || currentBallY < -currentHeight + Config::frameHeightCM)
    {
        currentBallY = rawYvalue + (Config::ballDiameterCM / 2 * (times + 1) * sign);
        currentHeight = Config::frameHeightCM * (times + 1);
        times++;
    }
    times--;

    // Get the new y-value.
    const float modifiedYvalue = rawYvalue + Config::ballDiameterCM * times * sign;

    // Calculate where the ball ends, when bouncing is included.
    float endValue;
    if (times % 2 == 0)
    {
        endValue = fmod(fmod(modifiedYvalue, Config::frameHeightCM) + Config::frameHeightCM, Config::frameHeightCM);
    }
    else
    {
        endValue = Config::frameHeightCM - fmod((fmod(modifiedYvalue, Config::frameHeightCM) + Config::frameHeightCM), Config::frameHeightCM);
    }

    return endValue;
}
