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
    bool     _enabled;         // stato generale del clock (abilitato/disabilitato)
    bool     _isOn;            // true = fase ON dell'onda, false = fase OFF
    uint32_t _phaseStartMs;    // momento in cui è iniziata l'attuale fase (ON o OFF)
    uint32_t _lastTickMs;      // ultimo istante in cui è stato rilevato il "tick" di periodo
    uint32_t _periodMs;        // periodo complessivo dell'onda in millisecondi
    uint32_t _onTimePeriod;    // durata della fase ON, ricavata da periodo + duty
    uint32_t _offTimePeriod;   // durata della fase OFF, ricavata da periodo + duty
    uint8_t  _dutyCyclePerc;   // ultima % di duty cycle ricevuta dall'utente (0–100)    

    // Converte una percentuale di duty (0–100) in un valore normalizzato [0.0–1.0],
    // applicando il clamp interno (es. min 10%, max 90%).
    float normalizedDuty(float dutyCyclePerc);

    // ricalcola _onTimePeriod e _offTimePeriod da _periodMs + _dutyCyclePerc
    // con valori normalizzati
    void updateTiming();

public:
    // Costruttore:
    // periodMs  -> periodo complessivo in millisecondi
    // dutyPerc  -> duty cycle in percentuale (0–100), default 50%
    ClockTimer(uint32_t periodMs, uint8_t dutyPerc = 50U);

    // Abilita il clock e sincronizza la fase iniziale rispetto al tempo "now".
    void start(uint32_t now);
    void start();

    // Disabilita il clock (l'onda viene "congelata" in stato OFF).
    void stop();

    // Aggiorna il periodo complessivo in millisecondi (ricalcola anche on/off).
    void setPeriodMs(uint32_t newPeriodMs);

    // Imposta il duty cycle in percentuale (0–100) e ricalcola on/off.
    void setDutyPerc(uint8_t newDutyCyclePerc);

    // Aggiorna lo stato dell’onda in base al tempo attuale.
    // Gestisce automaticamente le transizioni ON → OFF e OFF → ON.
    void update(uint32_t now);
    void update();

    // Ritorna true una volta ogni periodo completo (evento di "fine periodo").
    bool tick(uint32_t now);
    bool tick();

    // Ritorna true se il clock è abilitato.
    bool isEnabled() const;

    // Ritorna true se, allo stato attuale, l'onda è nella fase ON.
    bool isOn() const;
};
