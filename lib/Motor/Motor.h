#pragma once
#include <Arduino.h>

// L298N motor driver for the single JGA25-370 motor.
// Works on both the old (2.x) and new (3.x) Arduino-ESP32 core
// LEDC APIs automatically -- see the top of Motor.cpp.
class Motor {
public:
    Motor(uint8_t in1Pin, uint8_t in2Pin, uint8_t enaPin, uint8_t pwmChannel = 0);

    bool begin();
    void forward(uint8_t speed);   // speed: 0-255
    void reverse(uint8_t speed);
    void stop();                   // coast to a stop
    void brake();                  // active brake (both inputs high)
    void setSpeed(uint8_t speed);  // change speed, keep current direction

    int getSpeed() const { return _currentSpeed; }
    bool isForward() const { return _forward; }

private:
    uint8_t _in1, _in2, _ena, _pwmChannel;
    int _currentSpeed = 0;
    bool _forward = true;

    void writePwm(uint32_t duty);
};
