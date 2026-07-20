#include "Motor.h"

// ============================================================
//  Arduino-ESP32 core compatibility shim.
//
//  Core 2.x:  ledcSetup(channel, freq, res) + ledcAttachPin(pin, channel)
//             ledcWrite(channel, duty)
//  Core 3.x:  ledcAttach(pin, freq, res)
//             ledcWrite(pin, duty)
//
//  PlatformIO's official registry currently still tends to
//  install the older core by default (it hasn't been kept
//  current), while newer setups (e.g. the pioarduino platform)
//  install the newer one. Rather than gamble on which you have,
//  this detects it and calls the right functions either way.
//
//  If it ever guesses wrong for your setup, force it by adding
//  ONE of these to platformio.ini's build_flags:
//    -D ROBOT_FORCE_NEW_LEDC_API=1
//    -D ROBOT_FORCE_OLD_LEDC_API=1
// ============================================================
#if defined(ROBOT_FORCE_NEW_LEDC_API)
    #define ROBOT_LEDC_NEW_API 1
#elif defined(ROBOT_FORCE_OLD_LEDC_API)
    #define ROBOT_LEDC_NEW_API 0
#elif defined(ESP_ARDUINO_VERSION) && defined(ESP_ARDUINO_VERSION_VAL)
    #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
        #define ROBOT_LEDC_NEW_API 1
    #else
        #define ROBOT_LEDC_NEW_API 0
    #endif
#else
    #define ROBOT_LEDC_NEW_API 0
#endif

static const int PWM_FREQ_HZ = 5000;
static const int PWM_RESOLUTION_BITS = 8;

Motor::Motor(uint8_t in1Pin, uint8_t in2Pin, uint8_t enaPin, uint8_t pwmChannel)
    : _in1(in1Pin), _in2(in2Pin), _ena(enaPin), _pwmChannel(pwmChannel) {}

bool Motor::begin() {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);

#if ROBOT_LEDC_NEW_API
    ledcAttach(_ena, PWM_FREQ_HZ, PWM_RESOLUTION_BITS);
#else
    ledcSetup(_pwmChannel, PWM_FREQ_HZ, PWM_RESOLUTION_BITS);
    ledcAttachPin(_ena, _pwmChannel);
#endif

    stop();
    return true;
}

void Motor::writePwm(uint32_t duty) {
#if ROBOT_LEDC_NEW_API
    ledcWrite(_ena, duty);
#else
    ledcWrite(_pwmChannel, duty);
#endif
}

void Motor::forward(uint8_t speed) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    writePwm(speed);
    _currentSpeed = speed;
    _forward = true;
}

void Motor::reverse(uint8_t speed) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    writePwm(speed);
    _currentSpeed = speed;
    _forward = false;
}

void Motor::stop() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    writePwm(0);
    _currentSpeed = 0;
}

void Motor::brake() {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, HIGH);
    writePwm(0);
    _currentSpeed = 0;
}

void Motor::setSpeed(uint8_t speed) {
    if (_forward) forward(speed);
    else reverse(speed);
}
