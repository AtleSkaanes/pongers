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

    digitalWrite(_directionPin, LOW);
    digitalWrite(_stepPin, LOW);

    while (!digitalRead(firstStepButton))
    {
        digitalWrite(_stepPin, HIGH);
        delay(10);
        digitalWrite(_stepPin, LOW);
        delay(10);
    }

    digitalWrite(_directionPin, HIGH);
    step_t numSteps = 0;
    while (!digitalRead(lastStepButton))
    {
        digitalWrite(_stepPin, HIGH);
        delay(10);
        digitalWrite(_stepPin, LOW);
        delay(10);
        numSteps++;
    }

    _heightStepCount = numSteps;
    _currentStep = _heightStepCount;
}

void AIStepper::Move(float position, uint8_t rpm)
{
    static const float stepPosRatio = _heightStepCount / Config::frameHeightCM;

    _targetStep = static_cast<step_t>(stepPosRatio * position);
    _rpm = rpm;
}

void AIStepper::UpdatePos()
{
    static uint32_t prevTime = micros();
    const uint32_t deltatime = micros() - prevTime;

    const float deltaMinutes = deltatime * pow(10, -6) / 60.0f;
    const float deltaRounds = _rpm * deltaMinutes;

    const step_t maxSteps = abs(_currentStep - _targetStep);
    step_t deltaSteps = static_cast<step_t>(deltaRounds * _stepsPrRound);
    deltaSteps = min(deltaSteps, maxSteps);

    // Maybe flip?
    const uint8_t direction = _currentStep < _targetStep ? HIGH : LOW;
    digitalWrite(_directionPin, direction);

    for (step_t step = 0; step < deltaSteps; step++)
    {
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(750);
        digitalWrite(_stepPin, LOW);
        delayMicroseconds(750);
    }

    _currentStep += direction ? deltaSteps : -deltaSteps;
    prevTime = micros();
}
