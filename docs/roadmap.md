# ClockTimer – Roadmap (Minimal)

This roadmap outlines the planned evolution of ClockTimer, focusing on realistic,
incremental improvements for the next minor versions.

---

## v1.1.0 — Next Minor Release

- **Planned**

- Add accessor functions:
  - `getPeriodMs()`
  - `getDutyPerc()`
- Add helper API:
  - `setFrequencyHz()`
  - `setDuty0to1()`
- Improve example set:
  - LED dimming via variable duty cycle
  - Multi-clock synchronization example
- Extend CI matrix:
  - Add Arduino Mega
  - Add ESP8266

---

## v1.2.0 — Medium-Term Improvements

- **Planned**

- Optional phase callbacks:
  - `onEnterOnPhase()`
  - `onEnterOffPhase()`
- Optional auto-update mode (internal call to `millis()`).
- Minor internal optimizations for memory usage.

---

## Future Ideas (to be evaluated)

- Additional waveform modes (triangular, pulse patterns).
- Support for microsecond resolution using `micros()`.
- Extended scheduler-like helpers.
