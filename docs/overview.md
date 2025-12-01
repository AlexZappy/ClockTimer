# ClockTimer – Overview

ClockTimer is a lightweight utility class that provides a non-blocking software clock
based on Arduino’s `millis()` function.

It is designed to replace delay-based timing logic and to support multiple
independent timers with configurable period and duty cycle.

---

## Key concepts

### 1. Non-blocking timing

The library uses the standard `millis()` counter to compute elapsed time,
allowing the application loop to run freely without any delay.

### 2. Period

The period defines the full duration of a cycle in milliseconds.

Example:

- `period = 1000 ms` → cycle repeats every second.

### 3. Duty cycle

The duty cycle defines how long the clock stays "ON" during each cycle.

Example:

- `duty = 50%` → 500 ms ON, 500 ms OFF (for a 1000 ms period).

Internally, the duty cycle is clamped to 10–90% to avoid trivial states.

### 4. State transitions

ClockTimer implements an internal state machine with two phases:

- ON phase
- OFF phase

`update(now)` is responsible for switching between the two phases.

---

## Basic usage

```cpp
#include <ClockTimer.h>

ClockTimer clk(1000, 50);   // 1000 ms period, 50% duty

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    clk.start(millis());
}

void loop() {
    uint32_t now = millis();
    clk.update(now);

    digitalWrite(LED_BUILTIN, clk.isOn() ? HIGH : LOW);
}
```

## Period event (tick)

```cpp
tick(now) returns true once per full cycle:

if (clk.tick(now)) {
    Serial.println("One period elapsed.");
}
```

## Changing parameters at runtime

```cpp
clk.setPeriodMs(500);   // Half the original period
clk.setDutyPerc(75);    // Longer ON phase
```

## Example projects

See the `/examples` folder:

- `basic_clock_demo`

- `multi_clock_demo`

Both are compiled automatically in CI on:

- Arduino Uno

- ESP32 Dev Module
