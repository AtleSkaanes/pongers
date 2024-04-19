# 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino"
# 2 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino" 2
# 3 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\pongers.ino" 2


BallSensor *sensor1 = nullptr;
BallSensor *sensor2 = nullptr;

bool ballIsReturning = false;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Serial.println("\nSTARTING UP!");

    LaserSensor *sensors[2] = {nullptr, nullptr};
    pin_t shutDownPins[2] = {8, 9};

    delay(100);

    CreateSensors<2>(shutDownPins, sensors, SensorMode::HighSpeed);

    delay(100);

    // LaserSensor *sensor = &LaserSensor::Create(8, SensorMode::HighAccuracy);

    sensor1 = BallSensor::CreateHeap(34.5, *sensors[0]);
    sensor2 = BallSensor::CreateHeap(25.5, *sensors[1]);

    while (sensor1->GetMaxDistanceCM() <= 1.0f || sensor1->GetMaxDistanceCM() == Config::maxSensorDistanceCM || sensor1->GetMaxDistanceCM() != sensor1->GetMaxDistanceCM())
    {
        sensor1->CalibrateMaxDistance();
    }
    while (sensor2->GetMaxDistanceCM() <= 1.0f || sensor2->GetMaxDistanceCM() == Config::maxSensorDistanceCM || sensor1->GetMaxDistanceCM() != sensor1->GetMaxDistanceCM())
    {
        sensor2->CalibrateMaxDistance();
    }

    Serial.println("MAX DISTANCE:");
    Serial.print("SENSOR 1: ");
    Serial.println(sensor1->GetMaxDistanceCM());

    Serial.print("SENSOR 2: ");
    Serial.println(sensor2->GetMaxDistanceCM());
    Serial.println("\n");

    float minDist = (sensor1->GetMaxDistanceCM() < sensor2->GetMaxDistanceCM()) ? sensor1->GetMaxDistanceCM() : sensor2->GetMaxDistanceCM();
    sensor1->SetMaxDistanceCM(minDist);
    sensor2->SetMaxDistanceCM(minDist);

    delay(200);
    Serial.println("READY");
}

void loop()
{
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

    if (sensor2->HasDetected() && !sensor1->HasDetected())
    {
        Serial.println("ball returning");
        ballIsReturning = true;
    }

    if (sensor1->HasDetected() && ballIsReturning)
    {
        sensor1->Reset();
        sensor2->Reset();
        Serial.println("Ball returned; Resat sensors");
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

        Serial.print("FIRST DETECTED: ");
        Serial.println(sensor1->GetMeasuredDistanceCM());
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

        Serial.print("SECOND DETECTED: ");
        Serial.println(sensor2->GetMeasuredDistanceCM());
    }

    Serial.println("\nBALL MEASUREMENTS:");
    Serial.print("FIRST: ");
    Serial.println(sensor1->GetMeasuredDistanceCM());
    Serial.print("SECOND: ");
    Serial.println(sensor2->GetMeasuredDistanceCM());

    float endPoint = PredictEndPointCM(*sensor1, *sensor2);
    Serial.print("ENDPOINT: ");
    Serial.println(endPoint);

    delay(200);

    // sensor2->Reset();
    // sensor1->Reset();
}

float PredictEndPointCM(BallSensor sensor1, BallSensor sensor2)
{
    const float deltaHeigt = sensor2.GetMeasuredDistanceCM() - sensor1.GetMeasuredDistanceCM();
    const float deltaWidth = ((sensor1.GetXPositionCM() - sensor2.GetXPositionCM())>0?(sensor1.GetXPositionCM() - sensor2.GetXPositionCM()):-(sensor1.GetXPositionCM() - sensor2.GetXPositionCM()));

    // How many cm the ball goes up, for each cm it goes forwards
    const float slope = deltaHeigt / deltaWidth;

    // What y-coordinate (in cm), the ball SHOULD have, bouncing not included.
    const float rawYvalue = slope * sensor2.GetXPositionCM() + sensor2.GetMeasuredDistanceCM();

    // This value indicates if the balls slope is positve (+1), or negtaive (-1)
    const int sign = slope == 0 ? 1 : slope / ((slope)>0?(slope):-(slope));

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
