# ClockTimer – API Reference

This document provides the full technical reference for the `ClockTimer` class,
including constructor, methods, state behavior, and usage constraints.

---

## Class: `ClockTimer`

A lightweight, non-blocking software clock based on Arduino `millis()`.
Uses an internal ON/OFF state machine for periodic waveform generation.

## Constructor

### `ClockTimer(uint32_t periodMs, uint8_t dutyPerc = 50);`

Creates a new clock instance.

#### Arguments

- `periodMs`
  Total duration of the full cycle in milliseconds.

- `dutyPerc`
  Duty cycle in percent (0–100).
  Internally normalized and clamped to 10–90%.

#### Example

```cpp
ClockTimer clk(1000, 50);  // 1000 ms total period, 500 ms ON, 500 ms OFF
```

---

## Methods

### `void start(uint32_t now);`

Enables the clock and synchronizes its internal timing.

### `void start();`

Convenience overload → equivalent to `start(millis())`.

### `void stop();`

Disables the clock and forces OFF state.

### `void update(uint32_t now);`

Advances ON/OFF state based on time.

### `void update();`

Equivalent to `update(millis())`.

### `bool tick(uint32_t now);`

Returns true once per full cycle.

### `bool tick();`

Equivalent to `tick(millis())`.

### `void setPeriodMs(uint32_t newPeriodMs);`

Updates total period.

### `void setDutyPerc(uint8_t newDutyPerc);`

Updates duty cycle (clamped to 10–90%).

### `bool isEnabled() const;`

Returns whether the clock is active.

### `bool isOn() const;`

Returns ON/OFF state.

---

## Internal behavior summary

Duty normalization:

```cpp
mapped = duty / 100
mapped clamped to [0.1, 0.9]
```

Timing:

```cpp
onTime  = periodMs * mapped
offTime = periodMs - onTime
```

State machine:

- Starts in ON
- Switches ON→OFF after onTime
- Switches OFF→ON after offTime

Overflow safe due to unsigned arithmetic.
