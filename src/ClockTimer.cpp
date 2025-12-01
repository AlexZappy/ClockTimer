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


#include <ClockTimer.h>

// Constructor: initializes timer as disabled with the required period
ClockTimer::ClockTimer( uint32_t period, uint8_t dutyPerc):
    _enabled(false),
    _periodMs(period),
    _dutyCyclePerc(dutyPerc),
    _isOn(false),
    _phaseStartMs(0UL),
    _lastTickMs(0UL),
    _onTimePeriod(0UL),
    _offTimePeriod(0UL)
{
    updateTiming();
}

//=====================================================================
// private functions
//=====================================================================

// Must update _onTimePeriod and _offTimePeriod values
// _onTimePeriod = _periodMs * _dutyCyclePerc(normalized) so between 0.1..0.9
// _offTimePeriod = _periodMs - onTimePeriod
void ClockTimer::updateTiming(){
    float normVal = normalizedDuty(_dutyCyclePerc);
    _onTimePeriod = _periodMs * normVal;
    _offTimePeriod = _periodMs - _onTimePeriod;
}

// Must normalize value between 0.1 and 0.9 %
// so dutyCyclePerc / 100 --> value to round
float ClockTimer::normalizedDuty(float valuePerc)
{   // Calculate duty-cycle value still to clamp
    float mappedValue = (valuePerc / 100);
    // Clamping to avoid out-of-range values
    if (mappedValue < 0.1) mappedValue = 0.1;
    if (mappedValue > 0.9) mappedValue = 0.9;

    return mappedValue;
}

//=====================================================================
// public functions
//=====================================================================

// This goes in setup
void ClockTimer::start(uint32_t now)
{   // Activate timer and initialize lastTickMs
    _enabled = true;
    _isOn = true;
    _lastTickMs = now;
    _phaseStartMs = now;
}

void ClockTimer::start(){
    unsigned long now = millis();
    start(now);
}

void ClockTimer::stop()
{   // Stop timer
    _enabled = false;
    _isOn = false;
    _phaseStartMs = 0UL;
}

// Goes in LOOP. Example: 
// clock.update(millis());
// digitalWrite(LED, clock.isOn() ? HIGH : LOW);

void ClockTimer::update(uint32_t now)
{   // Check if we're in the ON or OFF phase of the wave
    // Compare elapsed time with _onTimePeriod or _offTimePeriod
    // Change phase when needed
    // Update _isOn and _phaseStartMs
    if (_enabled){
        uint32_t elapsed = now - _phaseStartMs; // Calculate elapsed time
        if (!_isOn)
        {
            if (elapsed >= _offTimePeriod){
                _isOn = true;
                _phaseStartMs = now;
            }
        }else
        {
            if (elapsed >= _onTimePeriod){
                _isOn = false;
                _phaseStartMs = now;
            }
        }
    }
}

void ClockTimer::update(){
    unsigned long now = millis();
    update(now);
}

// To be called in loop, e.g.: if (timer1.tick(millis())) { ... }
bool ClockTimer::tick(uint32_t _now)
{
    // Case 1: timer disabled → exit immediately
    if (!_enabled) 
        return false;

    // Case 2: period not expired → exit immediately
    if (_now - _lastTickMs < _periodMs) 
        return false;

    // Case 3: period expired → update and signal
    _lastTickMs = _now;
    return true;
}

bool ClockTimer::tick(){
    unsigned long now = millis();
    return tick(now);
}


// Set duty cycle percentage (0–100) and recalculate on/off.
void ClockTimer::setDutyPerc(uint8_t newDutyCyclePerc)
{
    _dutyCyclePerc = newDutyCyclePerc;
    updateTiming();
}

// Update overall period in milliseconds (also recalculates on/off).
void ClockTimer::setPeriodMs(uint32_t newPeriodMs)
{
    _periodMs = newPeriodMs;
    updateTiming();
}

// Return true if clock is enabled.
bool ClockTimer::isEnabled() const
{
 return _enabled;  
}

// Return true if, at current state, wave is in ON phase.
bool ClockTimer::isOn() const
{
    return _isOn;
}