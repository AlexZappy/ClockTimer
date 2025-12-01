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

#include <Arduino.h>
#include <ClockTimer.h>

// -----------------------------------------------------------------------------
// Example: multiple ClockTimer instances controlling different LEDs
// -----------------------------------------------------------------------------

// LED pins (change these according to your board layout)
const uint8_t LED1_PIN = 8;
const uint8_t LED2_PIN = 9;
const uint8_t LED3_PIN = 10;

// Clock 1: 1 Hz, 50% duty  -> "standard" blink
ClockTimer clk1(1000, 50);

// Clock 2: 2 Hz, 25% duty  -> faster, mostly OFF
ClockTimer clk2(500, 25);

// Clock 3: 0.5 Hz, 75% duty -> slow, mostly ON
ClockTimer clk3(2000, 75);

// Used to periodically reconfigure clock 3
uint32_t lastConfigChangeMs = 0;
const uint32_t CONFIG_INTERVAL_MS = 10000;  // change pattern every 10 seconds

void setup() {
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial) {
        ; // wait for Serial on some boards (e.g. native USB)
    }

    Serial.println("Multi ClockTimer demo started.");

    uint32_t now = millis();

    // Start all clocks at the same reference time
    clk1.start(now);
    clk2.start(now);
    clk3.start(now);

    lastConfigChangeMs = now;
}

void loop() {
    uint32_t now = millis();

    // -------------------------------------------------------------------------
    // 1) Update all clocks
    // -------------------------------------------------------------------------
    clk1.update(now);
    clk2.update(now);
    clk3.update(now);

    // -------------------------------------------------------------------------
    // 2) Drive LEDs according to each clock state
    // -------------------------------------------------------------------------
    digitalWrite(LED1_PIN, clk1.isOn() ? HIGH : LOW);
    digitalWrite(LED2_PIN, clk2.isOn() ? HIGH : LOW);
    digitalWrite(LED3_PIN, clk3.isOn() ? HIGH : LOW);

    // -------------------------------------------------------------------------
    // 3) Print events once per period for each clock
    // -------------------------------------------------------------------------
    if (clk1.tick(now)) {
        Serial.println("[clk1] Period elapsed (1 Hz, 50% duty).");
    }

    if (clk2.tick(now)) {
        Serial.println("[clk2] Period elapsed (2 Hz, 25% duty).");
    }

    if (clk3.tick(now)) {
        Serial.print("[clk3] Period elapsed (dynamic pattern).");
        // If you expose a getter for duty/period in the future, you can print it here.
        Serial.println();
    }

    // -------------------------------------------------------------------------
    // 4) Dynamically reconfigure clock 3 every CONFIG_INTERVAL_MS
    //    - This shows how to change period and duty at runtime
    // -------------------------------------------------------------------------
    if (now - lastConfigChangeMs >= CONFIG_INTERVAL_MS) {
        lastConfigChangeMs = now;

        static uint8_t mode = 0;
        mode = (mode + 1) % 3;  // cycle through 3 patterns

        switch (mode) {
            case 0:
                // Slow, mostly ON
                clk3.setPeriodMs(2000);   // 0.5 Hz
                clk3.setDutyPerc(75);     // 75% ON
                Serial.println("[clk3] Reconfigured: 0.5 Hz, 75% duty.");
                break;

            case 1:
                // Medium speed, symmetric
                clk3.setPeriodMs(1000);   // 1 Hz
                clk3.setDutyPerc(50);     // 50% ON
                Serial.println("[clk3] Reconfigured: 1 Hz, 50% duty.");
                break;

            case 2:
                // Faster, mostly OFF
                clk3.setPeriodMs(500);    // 2 Hz
                clk3.setDutyPerc(20);     // 20% ON
                Serial.println("[clk3] Reconfigured: 2 Hz, 20% duty.");
                break;
        }

          // Optional: resynchronize clock 3 phase to "now"
          // so the new configuration starts from this instant
          clk3.start(now);

    }

    // No delay: this example is fully non-blocking
}
