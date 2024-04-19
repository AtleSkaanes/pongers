#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\BallSensor.hpp"
#pragma once

#include "LaserSensor.hpp"
#include "typedefines.hpp"

class BallSensor
{
private:
    float _xPosition;
    bool _hasDetected;
    float _measuredDistance;
    float _maxDistanceCM;
    LaserSensor _sensor;

    BallSensor(float xPosition, LaserSensor sensor);

public:
    BallSensor() = default;

    static BallSensor Create(float xPosition, LaserSensor sensor);
    static BallSensor *CreateHeap(float xPosition, LaserSensor sensor);

    float MeasureDistanceCM();
    float CalibrateMaxDistance();

    void Reset();

    bool HasDetected() const;
    float GetMeasuredDistanceCM() const;
    void SetMeasuredDistanceCM(float distanceCM);
    float GetXPositionCM() const;
    float GetMaxDistanceCM() const;
    void SetMaxDistanceCM(float distanceCM);
    uint8_t GetLastStatus() const;

    operator bool();
};