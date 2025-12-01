# Changelog

All notable changes to this project will be documented in this file.

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] - 2025-12-01

### Added

- Initial public release of the **ClockTimer** library.
- Non-blocking period timing based on `millis()`.
- Internal ON/OFF state machine with configurable period and duty cycle.
- Automatic duty normalization and clamping (10%–90% effective range).
- `update()` method to advance timing state.
- `tick()` method for detecting end-of-period events.
- `start()` and `stop()` methods.
- `setPeriodMs()` and `setDutyPerc()` runtime configuration.
- Examples:
  - `clock_example` – basic use with a single clock.
  - `multi_clock_demo` – multiple clocks with dynamic reconfiguration.

### Notes

- This version is the first stable release and intended as a foundation for future extensions.

---

## [Unreleased]

### Planned

- Add optional edge detection (rising/falling) notification.
- Optional "phase reset" flag when changing duty or period.
- Add getters for duty and period.
- Add `invert()` method to swap ON/OFF behavior.
