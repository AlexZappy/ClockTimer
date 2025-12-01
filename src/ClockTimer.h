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

    // Converts a duty percentage (0–100) to a normalized value [0.0–1.0],
    // applying internal clamping (e.g., min 10%, max 90%).
    float normalizedDuty(float dutyCyclePerc);

    // Recalculates _onTimePeriod and _offTimePeriod from _periodMs + _dutyCyclePerc
    // using normalized values
    void updateTiming();

public:
    // Constructor:
    // periodMs  -> overall period in milliseconds
    // dutyPerc  -> duty cycle percentage (0–100), default 50%
    ClockTimer(uint32_t periodMs, uint8_t dutyPerc = 50U);

    // Enables the clock and synchronizes the initial phase relative to time "now".
    void start(uint32_t now);
    void start();

    // Disables the clock (wave is "frozen" in OFF state).
    void stop();

    // Updates the overall period in milliseconds (also recalculates on/off).
    void setPeriodMs(uint32_t newPeriodMs);

    // Sets the duty cycle percentage (0–100) and recalculates on/off.
    void setDutyPerc(uint8_t newDutyCyclePerc);

    // Updates the wave state based on current time.
    // Automatically handles ON → OFF and OFF → ON transitions.
    void update(uint32_t now);
    void update();

    // Returns true once every complete period ("end of period" event).
    bool tick(uint32_t now);
    bool tick();

    // Returns true if the clock is enabled.
    bool isEnabled() const;

    // Returns true if, at current state, the wave is in the ON phase.
    bool isOn() const;
};