#pragma once
#include <Arduino.h>

// Quadrature decoder for the JGA25-370's built-in Hall encoder.
class Encoder {
public:
    Encoder(uint8_t pinA, uint8_t pinB);

    bool begin();
    void update();               // call every loop; recomputes RPM periodically
    long getTicks() const { return _ticks; }
    float getRPM() const { return _rpm; }
    void reset();

    void handleInterrupt();      // public so the static ISR trampoline can call it

private:
    uint8_t _pinA, _pinB;
    volatile long _ticks = 0;
    volatile uint8_t _lastState = 0;
    float _rpm = 0;
    unsigned long _lastUpdateMs = 0;
    long _lastTicksSnapshot = 0;

    static void IRAM_ATTR isrHandler(void* arg);
};
