#include "Encoder.h"

// ============================================================
static const float TICKS_PER_REV = 5280.0f; // PLACEHOLDER -- calibrate me

Encoder::Encoder(uint8_t pinA, uint8_t pinB) : _pinA(pinA), _pinB(pinB) {}

bool Encoder::begin()
{
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    _lastState = (digitalRead(_pinA) << 1) | digitalRead(_pinB);

    attachInterruptArg(digitalPinToInterrupt(_pinA), isrHandler, this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(_pinB), isrHandler, this, CHANGE);

    _lastUpdateMs = millis();
    return true;
}

void IRAM_ATTR Encoder::isrHandler(void *arg)
{
    Encoder *self = static_cast<Encoder *>(arg);
    self->handleInterrupt();
}

void Encoder::handleInterrupt()
{
    uint8_t a = digitalRead(_pinA);
    uint8_t b = digitalRead(_pinB);
    uint8_t state = (a << 1) | b;

    // Standard quadrature transition table: +1 = forward step,
    // -1 = reverse step, 0 = no change or a skipped/bounced
    // transition (ignored rather than guessed, so a noisy edge
    // can't corrupt the count).
    static const int8_t transitionTable[16] = {
        0, 1, -1, 0,
        -1, 0, 0, 1,
        1, 0, 0, -1,
        0, -1, 1, 0};

    uint8_t index = (_lastState << 2) | state;
    _ticks += transitionTable[index];
    _lastState = state;
}

void Encoder::update()
{
    unsigned long now = millis();
    unsigned long elapsed = now - _lastUpdateMs;

    if (elapsed >= 100)
    {
        long deltaTicks = _ticks - _lastTicksSnapshot;
        float revolutions = deltaTicks / TICKS_PER_REV;
        float minutes = elapsed / 60000.0f;
        _rpm = revolutions / minutes;

        _lastTicksSnapshot = _ticks;
        _lastUpdateMs = now;
    }
}

void Encoder::reset()
{
    _ticks = 0;
    _lastTicksSnapshot = 0;
}
