# ClockTimer – Non-blocking software clock based on `millis()`

[![version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/AlexZappy/ClockTimer)
[![license](https://img.shields.io/badge/license-GPL--3.0-green.svg)](LICENSE)
[![arduino](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![platformio](https://img.shields.io/badge/PlatformIO-compatible-orange.svg)](https://platformio.org/)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Registry-yellow.svg)](https://registry.platformio.org/libraries/alexzappy/ClockTimer)
[![CI](https://github.com/AlexZappy/ClockTimer/actions/workflows/ci.yml/badge.svg)](https://github.com/AlexZappy/ClockTimer/actions/workflows/c.yml)

`ClockTimer` is a small utility class for Arduino that implements a **non-blocking periodic clock** with configurable period and duty cycle, using `millis()` as the time base.

It is designed to replace `delay()`-based blinking or timing logic with a clean, reusable API that scales well when multiple clocks are needed.

---

## Installation

### Arduino IDE

1. Download the library as ZIP  
2. Go to: `Sketch → Include Library → Add .ZIP Library…`  
3. Select the downloaded ZIP

### PlatformIO

Add this to your `platformio.ini`:

```ini
lib_deps =
    AlexZappy/ClockTimer
```

(If not yet available on the Registry, use a local `lib/ClockTimer` folder.)

---

## Features

- Uses `millis()` internally (non-blocking, cooperative timing).
- Configurable **period** (in milliseconds).
- Configurable **duty cycle** (percentage of ON time per period).
- Built-in **duty clamping** to avoid trivial 0% / 100% cases:
  - internal duty is limited to roughly 10–90%.
- Simple API:
  - `start()` / `start(now)`  
  - `update()` / `update(now)`  
  - `tick()` / `tick(now)`  
  - `isOn()` and `isEnabled()`
- Multiple independent instances can run in parallel.
- No dynamic allocation, suitable for small MCUs.

---

## Timing model

For a given period `T` (in milliseconds) and duty cycle `D` (in percent):

```cpp
onTime  = T * D_normalized
offTime = T - onTime
```

Where:

`D_normalized = D / 100.0`, then clamped into the range `[0.1, 0.9]`.

This means:

- Very small duty cycles (e.g. 2%) become ~10%
- Very large duty cycles (e.g. 99%) become ~90%

This avoids pathological cases where the output would be almost constantly OFF or ON.

---

## Basic usage

### 1. Include the header

```cpp
#include <ClockTimer.h>
```

### 2. Create an instance

```cpp
ClockTimer clk(1000, 50);   // period = 1000 ms (1 second), duty = 50%
```

### 3. Start the clock

```cpp
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    clk.start();     // or clk.start(millis());
}
```

### 4. Update in loop

```cpp
void loop() {
    clk.update();    // or clk.update(millis());
    digitalWrite(LED_BUILTIN, clk.isOn() ? HIGH : LOW);
}
```

---

## Period event (`tick()`)

To trigger actions once per period:

```cpp
void loop() {
    clk.update();

    if (clk.tick()) {
        Serial.println("Period elapsed");
    }
}
```

Notes:

- `tick()` returns `true` exactly once every `periodMs` milliseconds.
- Works only after `start()` has been called.

---

## API Reference

### Constructor

`ClockTimer(uint32_t periodMs, uint8_t dutyPerc = 50);`

### Methods

- `void start()`  
- `void start(uint32_t now)`  
- `void stop()`  
- `void update()`  
- `void update(uint32_t now)`  
- `bool tick()`  
- `bool tick(uint32_t now)`  
- `void setPeriodMs(uint32_t newPeriodMs)`  
- `void setDutyPerc(uint8_t newDutyCyclePerc)`  
- `bool isEnabled() const`  
- `bool isOn() const`  

---

## Multiple clocks example

See:

```text
examples/multi_clock_demo/multi_clock_demo.ino
examples/multi_clock_demo/multi_clock_demo.cpp
```

Demonstrates:

- Different periods and duty cycles
- Runtime reconfiguration
- Shared timestamp logic

---

## Notes and limitations

- `ClockTimer` relies on the standard Arduino `millis()` behavior (unsigned wrap-safe arithmetic).
- Very small periods (< 5–10 ms) may show jitter due to loop() latency.
- Input duty (0–100) is normalized and clamped to ~10–90%.

---

## License

This library is released under the GNU General Public License v3.0 (GPL-3.0).

A copy of the license is provided in the `LICENSE` file.
