#include "Encoder.h"

// ============================================================
//  TICKS_PER_REV -- you MUST calibrate this for your specific
//  motor/gearbox. The encoder disc gives ~11 pulses per MOTOR
//  shaft revolution (before the gearbox). In full quadrature
//  decoding (counting every edge on both channels, which is
//  what this driver does), that's 4 ticks per pulse = ~44 ticks
//  per motor-shaft revolution. After the gearbox: ticks per
//  OUTPUT-shaft revolution = 44 x (your gear ratio). JGA25-370
//  gearboxes are sold in many ratios (roughly 1:30 to 1:1000+),
//  so there's no single correct number here. The placeholder
//  below assumes a common ~1:120 gearbox. To find YOUR real
//  value:
//    1. Mark the output shaft (a bit of tape works).
//    2. Call reset(), then rotate the output shaft exactly one
//       full turn by hand.
//    3. getTicks() now holds your real TICKS_PER_REV -- replace
//       the number below with it.
// ============================================================
static const float TICKS_PER_REV = 5280.0f; // PLACEHOLDER -- calibrate me

Encoder::Encoder(uint8_t pinA, uint8_t pinB) : _pinA(pinA), _pinB(pinB) {}

bool Encoder::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    _lastState = (digitalRead(_pinA) << 1) | digitalRead(_pinB);

    attachInterruptArg(digitalPinToInterrupt(_pinA), isrHandler, this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(_pinB), isrHandler, this, CHANGE);

    _lastUpdateMs = millis();
    return true;
}

void IRAM_ATTR Encoder::isrHandler(void* arg) {
    Encoder* self = static_cast<Encoder*>(arg);
    self->handleInterrupt();
}

void Encoder::handleInterrupt() {
    uint8_t a = digitalRead(_pinA);
    uint8_t b = digitalRead(_pinB);
    uint8_t state = (a << 1) | b;

    // Standard quadrature transition table: +1 = forward step,
    // -1 = reverse step, 0 = no change or a skipped/bounced
    // transition (ignored rather than guessed, so a noisy edge
    // can't corrupt the count).
    static const int8_t transitionTable[16] = {
         0,  1, -1,  0,
        -1,  0,  0,  1,
         1,  0,  0, -1,
         0, -1,  1,  0
    };

    uint8_t index = (_lastState << 2) | state;
    _ticks += transitionTable[index];
    _lastState = state;
}

void Encoder::update() {
    unsigned long now = millis();
    unsigned long elapsed = now - _lastUpdateMs;

    if (elapsed >= 100) {
        long deltaTicks = _ticks - _lastTicksSnapshot;
        float revolutions = deltaTicks / TICKS_PER_REV;
        float minutes = elapsed / 60000.0f;
        _rpm = revolutions / minutes;

        _lastTicksSnapshot = _ticks;
        _lastUpdateMs = now;
    }
}

void Encoder::reset() {
    _ticks = 0;
    _lastTicksSnapshot = 0;
}
