#pragma once

#include <Arduino.h>

struct MotorData {
    float current;
    float temperature;
    float vibration;
    int rpm;
};

class HealthMonitor {
private:
    String status;
    bool fault;

public:
    void update(const MotorData &data);
    String getStatus();
    bool isFault();
};