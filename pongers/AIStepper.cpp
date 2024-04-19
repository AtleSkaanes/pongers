// stdlib headers
#include <Arduino.h>

// project headers
#include "Config.hpp"

// class header
#include "AIStepper.hpp"

AIStepper::AIStepper(pin_t stepPin, pin_t directionPin, step_t stepsPrRound, step_t currentStep, step_t targetStep, step_t heightStepCount, uint8_t rpm)
    : _stepPin{stepPin}, _directionPin{directionPin}, _stepsPrRound{stepsPrRound}, _currentStep{currentStep}, _targetStep{targetStep}, _heightStepCount{heightStepCount}, _rpm{rpm}
{
}

AIStepper AIStepper::Create(pin_t stepPin, pin_t DirectionPin, step_t stepsPrRound)
{
    pinMode(stepPin, OUTPUT);
    pinMode(DirectionPin, OUTPUT);

    return AIStepper{stepPin, DirectionPin, stepsPrRound, 0, 0, 0, 0};
}

void AIStepper::Calibrate(pin_t firstStepButton, pin_t lastStepButton)
{
    pinMode(firstStepButton, INPUT);
    pinMode(lastStepButton, INPUT);

    digitalWrite(_directionPin, HIGH);
    digitalWrite(_stepPin, LOW);

    while (!digitalRead(firstStepButton))
    {
        digitalWrite(_stepPin, HIGH);
        delay(9);
        digitalWrite(_stepPin, LOW);
        delay(9);
    }

    delay(1000);

    digitalWrite(_directionPin, LOW);
    step_t numSteps = 0;
    while (!digitalRead(lastStepButton))
    {
        digitalWrite(_stepPin, HIGH);
        delay(9);
        digitalWrite(_stepPin, LOW);
        delay(9);
        numSteps++;
    }

    _heightStepCount = numSteps;
    _currentStep = _heightStepCount;
    Serial.print(F("HIGHEST STEP COUNT:\t"));
    Serial.println(_heightStepCount);
}

void AIStepper::Move(float position, uint8_t rpm)
{
    position = constrain(position, Config::paddleWidth / 2, Config::frameHeightCM - Config::paddleWidth / 2);
    position = map(position, Config::paddleWidth / 2, Config::frameHeightCM - Config::paddleWidth / 2, 0, Config::frameHeightCM);

    static const float stepPosRatio = _heightStepCount / Config::frameHeightCM;

    _targetStep = static_cast<step_t>(stepPosRatio * position);
    _rpm = rpm;
    _isMoving = true;
}

void AIStepper::UpdatePos()
{
    static uint32_t prevTime = micros();
    // Update the prevTime when in standby
    if (!_isMoving)
    {
        prevTime = micros();
        return;
    }

    const uint32_t deltatime = micros() - prevTime;

    const float deltaMinutes = deltatime * pow(10, -6) / 60.0f;
    const float deltaRounds = _rpm * deltaMinutes;

    const step_t maxSteps = abs(_currentStep - _targetStep);
    step_t deltaSteps = static_cast<step_t>(deltaRounds * _stepsPrRound);
    deltaSteps = min(deltaSteps, maxSteps);

    if (deltaSteps == 0)
    {
        return;
    }

    // Serial.print(F("D T: "));
    // Serial.print(deltatime);
    // Serial.print(F("\tD M: "));
    // Serial.print(deltaMinutes);
    // Serial.print(F("\tD R: "));
    // Serial.print(deltaRounds);
    // Serial.print(F("\tD S: "));
    // Serial.println(deltaSteps);

    // Maybe flip?
    const uint8_t direction = _currentStep < _targetStep ? LOW : HIGH;
    digitalWrite(_directionPin, direction);

    for (step_t step = 0; step < deltaSteps; step++)
    {
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(400);
        digitalWrite(_stepPin, LOW);
        delayMicroseconds(400);
    }

    _currentStep += direction ? -deltaSteps : deltaSteps;
    if (_currentStep == _targetStep)
        _isMoving = false;

    Serial.print(F("CURRENT STEP: "));
    Serial.print(_currentStep);
    Serial.print(F("\tCurrent CM: "));
    Serial.print(_currentStep * (Config::frameHeightCM / _heightStepCount));
    Serial.println();

    prevTime = micros();
}
