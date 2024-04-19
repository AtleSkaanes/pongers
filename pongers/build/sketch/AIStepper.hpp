#line 1 "C:\\Users\\atles\\Documents\\Dev\\Skole\\Elektronik\\Pongers\\pongers\\AIStepper.hpp"
#pragma once
// stdlib headers
#include <stdint.h>

// Project headers
#include "Config.hpp"
#include "typedefines.hpp"

class AIStepper
{
private:
    pin_t _stepPin;
    pin_t _directionPin;
    step_t _stepsPrRound;
    step_t _currentStep;
    step_t _targetStep;
    step_t _heightStepCount;
    uint8_t _rpm;

    bool _isMoving = false;

    AIStepper(pin_t stepPin, pin_t directionPin, step_t stepsPrRound, step_t currentStep, step_t targetStep, step_t heightStepCount, uint8_t rpm);

public:
    static AIStepper Create(pin_t stepPin, pin_t DirectionPin, step_t stepsPrRound);
    void Calibrate(pin_t firstStepButton, pin_t lastStepButton);
    void Move(float position, uint8_t rpm);
    void UpdatePos();

    AIStepper &operator=(const AIStepper &) = default;

    AIStepper() = default;
};
