# ClockTimer – Internals

This document describes the internal architecture of the `ClockTimer` class:
timing model, state machine, duty-cycle normalization, and update flow.

---

## 1. Core architecture

ClockTimer is built around three core ideas:

1. A **monotonic time source** (`millis()`).
2. An **ON/OFF finite state machine**.
3. A **deterministic period boundary tracker** (`tick()`).

There is no dynamic allocation and no hardware interaction.

---

## 2. Internal variables

```cpp
_enabled         // clock active state
_isOn            // ON/OFF phase
_phaseStartMs    // timestamp of current phase start
_lastTickMs      // last period boundary
_periodMs        // full period (ms)
_onTimePeriod    // ms spent in ON
_offTimePeriod   // ms spent in OFF
_dutyCyclePerc   // 0–100 user input (later normalized)
```

All arithmetic uses **unsigned integers** to remain safe across millis overflow.

---

## 3. Duty cycle normalization

User input: `0–100%`

Internal normalized value:

```cpp
mapped = duty / 100.0
mapped clamped to [0.1, 0.9]
```

Motivation:

- Avoid trivial “always OFF” (0%) or “always ON” (100%).
- Ensure that both phases exist.

---

## 4. Phase timing computation

```cpp
onTimePeriod  = periodMs * mapped
offTimePeriod = periodMs - onTimePeriod
```

The sum always matches `periodMs`.

---

## 5. Phase switching logic

Executed inside `update(now)`:

```cpp
elapsed = now - phaseStartMs

if isOn:
    if elapsed >= onTimePeriod:
        isOn = false
        phaseStartMs = now
else:
    if elapsed >= offTimePeriod:
        isOn = true
        phaseStartMs = now
```

Notes:

- Behavior is deterministic.
- No blocking instructions.
- Overflow safe.

---

## 6. Period boundary logic (`tick()`)

```cpp
if !enabled → return false
if now - lastTickMs < periodMs → return false

lastTickMs = now
return true
```

Tick is independent of ON/OFF phase, and fires exactly once per full period.

---

## 7. Overflow behavior

Arduino `millis()` overflows every ~49 days.

Unsigned subtraction ensures correctness:

```cpp
(now - previous)   // correct even after overflow
```

This makes ClockTimer overflow-proof.

---

## 8. Interaction with external code

Typical update flow inside `loop()`:

```cpp
uint32_t now = millis();
clk.update(now);

if (clk.tick(now)) {
    // periodic event
}

digitalWrite(pin, clk.isOn() ? HIGH : LOW);
```

No timing drift occurs as long as `update()` is called regularly.
