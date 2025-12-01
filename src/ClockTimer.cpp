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

// Costruttore: inizializza il timer disattivato con il periodo richiesto
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
// funzioni private
//=====================================================================

// deve aggiornare i valori di _onTimePeriod e _offTimePeriod
// _onTimePeriod = _periodMs * _dutyCyclePerc(normalizzato) quindi da 0.1..0.9
// _offTimePeriod = _periodMs - onTimePeriod
void ClockTimer::updateTiming(){
    float normVal = normalizedDuty(_dutyCyclePerc);
    _onTimePeriod = _periodMs * normVal;
    _offTimePeriod = _periodMs - _onTimePeriod;
}

// deve normalizzare il valore tra 0.1 e 0.9 %
// quindi dutyCyclePerc / 100 --> valore da arrotondare
float ClockTimer::normalizedDuty(float valuePerc)
{   // calcolo valore di duty-cycle ancora da clampare
    float mappedValue = (valuePerc / 100);
    // Clamping per evitare fuori range
    if (mappedValue < 0.1) mappedValue = 0.1;
    if (mappedValue > 0.9) mappedValue = 0.9;

    return mappedValue;
}

//=====================================================================
// funzioni pubbliche
//=====================================================================

// in setup va questa
void ClockTimer::start(uint32_t now)
{   // attivo il timer e inizializzo lastTickMs
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
{   // fermo il timer
    _enabled = false;
    _isOn = false;
    _phaseStartMs = 0UL;
}

// va in LOOP ES. 
// clock.update(millis());
// digitalWrite(LED, clock.isOn() ? HIGH : LOW);

void ClockTimer::update(uint32_t now)
{   // vedo se sono nella fase ON o OFF dell'onda
    // confronto il temnpo trascorso con  _onTimePeriod o _offTimePeriod
    // cambio la fase quando serve
    // aggiorno _isOn e _phaseStartMs
    if (_enabled){
        uint32_t elapsed = now - _phaseStartMs; // calcolo il tempo trascorso
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

// Da richiamare in loop, es.: if (timer1.tick(millis())) { ... }
bool ClockTimer::tick(uint32_t _now)
{
    // Caso 1: timer disattivato → esci subito
    if (!_enabled) 
        return false;

    // Caso 2: periodo non scaduto → esci subito
    if (_now - _lastTickMs < _periodMs) 
        return false;

    // Caso 3: periodo scaduto → aggiorna e segnala
    _lastTickMs = _now;
    return true;
}

bool ClockTimer::tick(){
    unsigned long now = millis();
    return tick(now);
}


// Imposta il duty cycle in percentuale (0–100) e ricalcola on/off.
void ClockTimer::setDutyPerc(uint8_t newDutyCyclePerc)
{
    _dutyCyclePerc = newDutyCyclePerc;
    updateTiming();
}

// Aggiorna il periodo complessivo in millisecondi (ricalcola anche on/off).
void ClockTimer::setPeriodMs(uint32_t newPeriodMs)
{
    _periodMs = newPeriodMs;
    updateTiming();
}

// Ritorna true se il clock è abilitato.
bool ClockTimer::isEnabled() const
{
 return _enabled;  
}

// Ritorna true se, allo stato attuale, l'onda è nella fase ON.
bool ClockTimer::isOn() const
{
    return _isOn;
}    
