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
ClockTimer::ClockTimer(uint32_t period, uint8_t dutyPerc) :
    _enabled(false),
    _isOn(false),
    _phaseStartMs(0UL),
    _lastTickMs(0UL),
    _periodMs(period),
    _onTimePeriod(0UL),
    _offTimePeriod(0UL),
    _dutyCyclePerc(dutyPerc),
    _normVal(0.0f)
{
    updateTiming();
}

//=====================================================================
// private functions
//=====================================================================

// Updates _onTimePeriod and _offTimePeriod based on current period and duty cycle
// Calculates ON time as: period × normalized duty (clamped between 0.1 and 0.9)
// Calculates OFF time as: period - ON time
void ClockTimer::updateTiming(){
    _normVal = normalizedDuty(_dutyCyclePerc);  // Store normalized value
    _onTimePeriod = static_cast<uint32_t>(_periodMs * _normVal);
    _offTimePeriod = _periodMs - _onTimePeriod;
}

// Normalizes duty cycle percentage to a value between 0.1 and 0.9
// Converts percentage (0-100) to decimal (0.0-1.0) and applies clamping
// Returns: normalized value between 0.1 and 0.9 inclusive
float ClockTimer::normalizedDuty(float Perc)
{   
    float normalized = Perc / 100.0f;  // Convert percentage to decimal
    
    // Apply clamping to ensure duty cycle stays within operational bounds
    if (normalized < 0.1f) normalized = 0.1f;
    if (normalized > 0.9f) normalized = 0.9f;
    
    return normalized;  // Return clamped value
}

//=====================================================================
// public functions
//=====================================================================

// Starts the timer with explicit timestamp, enabling it and resetting phase
// Parameters: now - current timestamp in milliseconds
void ClockTimer::start(uint32_t now)
{   
    _enabled = true;
    _isOn = true;
    _lastTickMs = now;
    _phaseStartMs = now;
}

// Starts the timer using current system time from millis()
void ClockTimer::start(){
    uint32_t now = millis();
    start(now);
}

// Stops the timer, disabling further state updates
// Resets phase tracking and forces OFF state
void ClockTimer::stop()
{   
    _enabled = false;
    _isOn = false;
    _phaseStartMs = 0UL;
}

// Updates timer state based on current timestamp
// Manages transitions between ON and OFF phases based on calculated timings
// Parameters: now - current timestamp in milliseconds
void ClockTimer::update(uint32_t now)
{   
    if (_enabled){
        uint32_t elapsed = now - _phaseStartMs; // Calculate time since last phase change
        
        if (!_isOn)  // Currently in OFF phase
        {
            if (elapsed >= _offTimePeriod){  // OFF time completed
                _isOn = true;
                _phaseStartMs = now;  // Reset phase timer
            }
        }
        else  // Currently in ON phase
        {
            if (elapsed >= _onTimePeriod){  // ON time completed
                _isOn = false;
                _phaseStartMs = now;  // Reset phase timer
            }
        }
    }
}

// Updates timer state using current system time from millis()
void ClockTimer::update(){
    uint32_t now = millis();
    update(now);
}

// Checks if a complete period has elapsed since last tick
// Returns: true if period completed, false otherwise
// Parameters: _now - current timestamp in milliseconds
bool ClockTimer::tick(uint32_t _now)
{
    // Case 1: timer disabled → exit immediately
    if (!_enabled) 
        return false;

    // Case 2: period not expired → exit immediately
    if (_now - _lastTickMs < _periodMs) 
        return false;

    // Case 3: period expired → update last tick time and signal completion
    _lastTickMs = _now;
    return true;
}

// Checks for period completion using current system time from millis()
bool ClockTimer::tick(){
    uint32_t now = millis();
    return tick(now);
}

// Sets new duty cycle percentage and recalculates timing
// Parameters: newDutyCyclePerc - duty cycle as percentage (0-100)
void ClockTimer::setDutyPerc(uint8_t newDutyCyclePerc)
{
    _dutyCyclePerc = newDutyCyclePerc;
    updateTiming();
}

// Sets new overall period and recalculates timing
// Parameters: newPeriodMs - complete cycle period in milliseconds
void ClockTimer::setPeriodMs(uint32_t newPeriodMs)
{
    if (newPeriodMs == 0) {
        _periodMs = 1;  // o un valore minimo sensato, es: 10
    } else {
        _periodMs = newPeriodMs;
    }
    updateTiming();
}

// Checks if timer is currently enabled
// Returns: true if enabled, false if stopped
bool ClockTimer::isEnabled() const
{
    return _enabled;  
}

// Checks current phase of the timer
// Returns: true if in ON phase, false if in OFF phase
bool ClockTimer::isOn() const
{
    return _isOn;
}

//=====================================================================
// Getter functions for debugging and monitoring
//=====================================================================

// Returns the user-set duty cycle percentage (0-100)
// Note: Internally clamped to effective range via normalizedDuty()
uint8_t ClockTimer::getDutyPerc() const 
{
    return _dutyCyclePerc;
}

// Returns the normalized duty cycle value (0.1 to 0.9)
// This is the actual value used for timing calculations after clamping
float ClockTimer::getNormalizedDuty() const
{
    return _normVal;
}

// Returns calculated OFF phase duration in milliseconds
// Derived from: period × (1 - normalized duty)
uint32_t ClockTimer::getOffTime() const
{
    return _offTimePeriod;
}

// Returns calculated ON phase duration in milliseconds
// Derived from: period × normalized duty
uint32_t ClockTimer::getOnTime() const
{
    return _onTimePeriod;
}

// Returns overall timer period in milliseconds
// Complete cycle time: ON time + OFF time
uint32_t ClockTimer::getPeriodMs() const
{
    return _periodMs;
}