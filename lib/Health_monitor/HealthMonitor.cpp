#include "HealthMonitor.h"
#include "config.h"

void HealthMonitor::update(const MotorData &data)
{
    fault = false;

    if (data.current > SAFE_CURRENT_LIMIT_A)
    {
        status = "Overcurrent";
        fault = true;
    }
    else if (data.temperature > SAFE_TEMP_LIMIT_C)
    {
        status = "Overheating";
        fault = true;
    }
    else if (data.rpm < 50 && data.current > 1.5)
    {
        status = "Motor Stalled";
        fault = true;
    }
    else if (data.vibration > 2.5) // You can tune this later
    {
        status = "High Vibration";
        fault = true;
    }
    else
    {
        status = "Healthy";
    }
}

String HealthMonitor::getStatus()
{
    return status;
}

bool HealthMonitor::isFault()
{
    return fault;
}