/*
 * This file is part of the ClockTimer library.
 * Copyright (c) 2025 Alex Zappaterra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once
#include <Arduino.h>

// ============================================================================
// CLASS ClockTimer
// ============================================================================
class ClockTimer {
private:
    bool     _enabled;         // general clock state (enabled/disabled)
    bool     _isOn;            // true = ON phase of wave, false = OFF phase
    uint32_t _phaseStartMs;    // moment when the current phase (ON or OFF) started
    uint32_t _lastTickMs;      // last instant when the period "tick" was detected
    uint32_t _periodMs;        // overall wave period in milliseconds
    uint32_t _onTimePeriod;    // ON phase duration, derived from period + duty
    uint32_t _offTimePeriod;   // OFF phase duration, derived from period + duty
    uint8_t  _dutyCyclePerc;   // last duty cycle percentage received from user (0–100)
    float    _normVal;         // normalized duty value (0.1 to 0.9) after clamping

    // Converts a duty percentage (0–100) to a normalized value [0.0–1.0],
    // applying internal clamping (e.g., min 10%, max 90%).
    // Returns: clamped normalized value between 0.1 and 0.9
    float normalizedDuty(float dutyCyclePerc);

    // Recalculates _onTimePeriod and _offTimePeriod from _periodMs + _dutyCyclePerc
    // using normalized values. Called automatically when period or duty changes.
    void updateTiming();

public:
    // Constructor:
    // periodMs  -> overall period in milliseconds
    // dutyPerc  -> duty cycle percentage (0–100), default 50%
    // Timer starts disabled; call start() to begin operation.
    ClockTimer(uint32_t periodMs, uint8_t dutyPerc = 50U);

    // Enables the clock and synchronizes the initial phase relative to time "now".
    void start(uint32_t now);
    void start();  // Uses current millis() value

    // Disables the clock (wave is "frozen" in OFF state).
    // Resets phase tracking and forces OFF state.
    void stop();

    // Updates the overall period in milliseconds (also recalculates on/off).
    void setPeriodMs(uint32_t newPeriodMs);

    // Sets the duty cycle percentage (0–100) and recalculates on/off.
    void setDutyPerc(uint8_t newDutyCyclePerc);

    // Updates the wave state based on current time.
    // Automatically handles ON → OFF and OFF → ON transitions.
    void update(uint32_t now);
    void update();  // Uses current millis() value

    // Returns true once every complete period ("end of period" event).
    // Useful for periodic tasks without continuous state tracking.
    bool tick(uint32_t now);
    bool tick();  // Uses current millis() value

    // Returns true if the clock is enabled.
    bool isEnabled() const;

    // Returns true if, at current state, the wave is in the ON phase.
    bool isOn() const;

    // ========================================================================
    // Getter functions for debugging and monitoring
    // ========================================================================

    // Returns the overall period of the timer in milliseconds.
    // This is the complete cycle time (ON time + OFF time).
    uint32_t getPeriodMs() const;

    // Returns the user-set duty cycle percentage (0–100).
    // Note: Effective duty cycle is clamped internally via normalizedDuty().
    uint8_t getDutyPerc() const;

    // Returns calculated ON phase duration in milliseconds.
    // Derived from: period × normalized duty (clamped 0.1–0.9).
    uint32_t getOnTime() const;

    // Returns calculated OFF phase duration in milliseconds.
    // Derived from: period × (1 - normalized duty).
    uint32_t getOffTime() const;

    // Returns the normalized duty cycle value (0.1 to 0.9).
    // This is the actual value used for timing calculations after clamping.
    float getNormalizedDuty() const;
};