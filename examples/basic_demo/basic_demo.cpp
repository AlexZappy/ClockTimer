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

// Example: 1 Hz clock with 50% duty cycle
ClockTimer clk(1000, 50);

// Use a fixed LED pin (on many ESP32 dev boards it's GPIO 2)
const uint8_t LED_PIN = 2;

void setup() {
    pinMode(LED_PIN, OUTPUT);

    // Start the clock using the current millis()
    clk.start();

    Serial.begin(115200);
    Serial.println("ClockTimer example started");
}

void loop() {

    // Update internal ON/OFF state
    clk.update();

    // Apply wave output to the LED
    digitalWrite(LED_PIN, clk.isOn() ? HIGH : LOW);

    // Optional: event fired once per period
    if (clk.tick()) {
        Serial.println("Period complete");
    }
}

