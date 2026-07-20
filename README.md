# ESP32-S3 Autonomous Robot (JGA25-370)

A modular PlatformIO firmware project for an ESP32-S3-N16R8 DevKitC-1
driving one JGA25-370 gear motor (with encoder) through an L298N, with
an ADXL345 accelerometer, ACS712 current sensor, DS18B20 temperature
sensor, and an SD card logger.

Every sensor/actuator lives in its own folder under `lib/`, each with
its own standalone test sketch under `src/tests/`, so you can build and
upload just one component at a time before combining everything through
the `Robot` class in `src/`. `src/main.cpp` never changes — you extend
behavior in `src/Robot.cpp` and turn modules on/off in `include/config.h`.

## Wiring

| Component | Signal | ESP32-S3 Pin |
|---|---|---|
| ADXL345 | SDA / SCL | GPIO8 / GPIO9 |
| DS18B20 | Data (needs 4.7kΩ pull-up to 3.3V) | GPIO4 |
| ACS712 | OUT | GPIO1 |
| L298N | IN1 / IN2 / ENA | GPIO5 / GPIO6 / GPIO7 |
| JGA25-370 encoder | Yellow(A) / Green(B) | GPIO15 / GPIO16 |
| SD card module | CS / SCK / MISO / MOSI | GPIO10 / GPIO12 / GPIO13 / GPIO11 |

Pins were chosen to avoid the S3's strapping pins (0, 3, 45, 46),
native-USB pins (19, 20), and the pins reserved by the N16R8's octal
PSRAM (26-37).

**Hardware safety note:** the ACS712 outputs 0–5V, but the ESP32's ADC
only tolerates 3.3V. If your ACS712 module is powered from 5V, add a
voltage divider (e.g. 10kΩ/20kΩ) on its OUT pin before it reaches
GPIO1 — otherwise you risk damaging that ADC pin. Powering the ACS712
from 3.3V instead also works; check its datasheet for the resulting
mV/A sensitivity at 3.3V.

## Setup

1. VS Code → PlatformIO icon → New Project → board **"Espressif
   ESP32-S3-DevKitC-1"**, framework **Arduino**.
2. Replace the generated `platformio.ini` and `src/` with the contents
   of this project (or just drop this whole folder in as the project).
3. Bottom status bar → click the environment name → pick **robot** for
   the full system, or a **test_...** environment to test one
   component in isolation first (recommended order: adxl345, acs712,
   ds18b20, motor, encoder, sdcard — test each before wiring the next).

## What's automatic vs. what you calibrate

**Automatic, zero input needed:**
- ACS712 zero-current calibration on every boot
- Non-blocking sensor scheduling (nothing uses `delay()` in the main
  robot loop, so nothing stalls anything else)
- Continuous SD logging to `/robot_log.csv`
- Motor auto-shutoff the instant current or temperature crosses the
  limits in `include/config.h` — this is the "smart" safety layer

**You calibrate once:**
- `TICKS_PER_REV` in `lib/Encoder/Encoder.cpp` — depends on your
  specific gearbox ratio. Instructions are in the comment right above
  the constant (mark the shaft, spin it one full turn by hand, read
  back the tick count).
- `ACS712_MV_PER_AMP` in `include/config.h` if your sensor isn't the
  20A variant (185 for 5A, 66 for 30A).
- `SAFE_CURRENT_LIMIT_A` / `SAFE_TEMP_LIMIT_C` in `include/config.h` —
  tune these to your actual motor and battery.

**Not yet built:** anything that decides *where to go*. This
foundation reads sensors, logs data, and protects the motor, but has
no navigation or perception driving movement decisions yet. That logic
belongs in `src/Robot.cpp` once you know the actual task — line
following, obstacle avoidance, remote control with a safety override,
or a stationary monitoring rig are all different additions on top of
this same base.

## If something doesn't compile

This project's `lib/Motor/Motor.cpp` auto-detects whether your
installed Arduino-ESP32 core is the older 2.x line or the newer 3.x
line and calls the matching PWM API either way — you shouldn't need to
do anything here. If you ever do hit an LEDC-related build error,
force it manually by adding one line to `platformio.ini`'s
`build_flags`:

```ini
build_flags =
    -DBOARD_HAS_PSRAM
    -D ROBOT_FORCE_NEW_LEDC_API=1   ; or ROBOT_FORCE_OLD_LEDC_API=1
```
