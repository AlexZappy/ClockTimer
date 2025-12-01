# ClockTimer Library

[![version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/AlexZappy/ClockTimer)
[![license](https://img.shields.io/badge/license-GPL--3.0-green.svg)](LICENSE)
[![arduino](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![platformio](https://img.shields.io/badge/PlatformIO-compatible-orange.svg)](https://platformio.org/)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Registry-yellow.svg)](https://registry.platformio.org/libraries/alexzappy/ClockTimer)
[![CI](https://github.com/AlexZappy/ClockTimer/actions/workflows/ci.yml/badge.svg)](https://github.com/AlexZappy/ClockTimer/actions/workflows/c.yml)

## Overview

ClockTimer is an Arduino library for precise timing and waveform generation. It provides two operational modes: **continuous PWM-like output** and **periodic event signaling**, making it ideal for embedded applications requiring timed operations.

## Features

- **Dual operational modes**:
  - Continuous state management (ON/OFF phases) for PWM, LED breathing, motor control
  - Periodic event signaling for scheduled tasks, sensor polling, state machines
- **Runtime parameter adjustment** (period, duty cycle)
- **Automatic overflow handling** for millis() rollover
- **Lightweight and efficient** - suitable for resource-constrained MCUs
- **Thread-safe** (no interrupts or blocking operations)

## Installation

### Arduino IDE

1. Download the latest release from GitHub
2. In Arduino IDE: Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps = 
    https://github.com/AlexZappy/ClockTimer.git
```

or:

```ini
lib_deps =
    AlexZappy/ClockTimer
```

(If not yet available on the Registry, use a local `lib/ClockTimer` folder.)

---

## Quick Start

### Mode 1: Continuous Output (PWM-like)

```cpp
#include <ClockTimer.h>

ClockTimer timer(1000, 30); // 1s period, 30% duty cycle
const int LED_PIN = 13;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    timer.start();
}

void loop() {
    timer.update();                // Update internal state
    digitalWrite(LED_PIN, timer.isOn() ? HIGH : LOW);
}
```

### Mode 2: Periodic Eventsù

```cpp
#include <ClockTimer.h>

ClockTimer sensorTimer(5000); // Check sensor every 5 seconds

void setup() {
    sensorTimer.start();
}

void loop() {
    if (sensorTimer.tick()) {  // Returns true every 5 seconds
        readSensor();
        transmitData();
    }
}
```

## Use Cases & Applications

### LED Control

- **Breathing effects**: Smooth fade in/out with adjustable period
- **Pattern generation**: Morse code, SOS signals, attention patterns
- **Status indicators**: Heartbeat, error codes, battery status

```cpp
// Breathing LED example
ClockTimer breath(2000, 10); // 2s breath cycle, 10-90% duty

void setup() {
    ledcAttachPin(LED_PIN, 0);  // ESP32 PWM
    breath.start();
}

void loop() {
    breath.update();
    if (breath.isOn()) {
        // Add your PWM logic here
    }
}
```

### Motor & Actuator Control

- **PWM generation** for DC motor speed control
- **Servo pulse timing** (microsecond precision available)
- **Solenoid/valve actuation** with precise ON/OFF timing

```cpp
// Motor controller with soft start
ClockTimer motorTimer(100, 0); // Start at 0% duty

void accelerateMotor() {
    for (int duty = 0; duty <= 100; duty += 10) {
        motorTimer.setDutyPerc(duty);
        delay(200);
    }
}
```

### Sensor & Communication

- **Polling intervals** for sensors (I2C, SPI, analog)
- **Debounce timing** for buttons and switches
- **Data transmission windows** for LoRa, Bluetooth

```cpp
// Multi-rate sensor polling
ClockTimer fastSensor(100);   // 100ms for temperature
ClockTimer slowSensor(5000);  // 5s for humidity

void loop() {
    if (fastSensor.tick()) readTemperature();
    if (slowSensor.tick()) {
        readHumidity();
        publishToMQTT();
    }
}
```

### Audio & Signal Generation

- **Buzzer patterns** for alarms, notifications
- **Tone generation** with variable frequency/duty
- **MIDI clock synchronization**

```cpp
// Buzzer alarm pattern
ClockTimer beepPattern(500, 20); // 500ms, 20% duty (short beep)

void loop() {
    beepPattern.update();
    digitalWrite(BUZZER_PIN, beepPattern.isOn());
    
    if (alarmCondition) {
        beepPattern.setPeriodMs(100); // Faster beeping
        beepPattern.setDutyPerc(50);  // Longer beeps
    }
}
```

### State Machines & Automation

- **Timed state transitions**
- **Sequence control** for manufacturing/robotics
- **Safety timeouts** and watchdogs

```cpp
enum State { IDLE, HEATING, COOLING, ERROR };
State currentState = IDLE;
ClockTimer stateTimer;

void handleStateMachine() {
    switch (currentState) {
        case HEATING:
            if (stateTimer.tick()) {
                currentState = COOLING;
                stateTimer.setPeriodMs(30000); // 30s cooling
                stateTimer.start();
            }
            break;
        // ... other states
    }
}
```

## Advanced Usage

### Dynamic Parameter Adjustment

```cpp
ClockTimer adaptiveTimer(1000, 50);

void adjustBasedOnInput(int sensorValue) {
    // Map sensor value to period (100ms to 5000ms)
    uint32_t newPeriod = map(sensorValue, 0, 1023, 100, 5000);
    adaptiveTimer.setPeriodMs(newPeriod);
    
    // Inverse relationship for duty cycle
    uint8_t newDuty = map(sensorValue, 0, 1023, 90, 10);
    adaptiveTimer.setDutyPerc(newDuty);
}
```

### Multiple Synchronized Timers

```cpp
ClockTimer master(1000);
ClockTimer slave1(500);
ClockTimer slave2(2000);

void setup() {
    uint32_t now = millis();
    master.start(now);
    slave1.start(now);
    slave2.start(now);
}

void loop() {
    uint32_t now = millis();
    master.update(now);
    slave1.update(now);
    slave2.update(now);
    
    // Synchronized operations
    if (master.isOn()) {
        // Execute during master's ON phase
    }
}
```

### Microsecond Precision (Extended Class)

```cpp
class ClockTimerMicros : public ClockTimer {
public:
    void start() { start(micros()); }
    void update() { update(micros()); }
    bool tick() { return tick(micros()); }
    
    void setPeriodUs(uint32_t us) { setPeriodMs(us / 1000); }
    uint32_t getPeriodUs() { return _periodMs * 1000; }
};

ClockTimerMicros preciseTimer;
```

## API Reference

### Constructor

```cpp
ClockTimer(uint32_t periodMs, uint8_t dutyPerc = 50)
```

### Core Methods

| Method | Description | Time Complexity |
|--------|-------------|-----------------|
| `start()` / `start(now)` | Begin timing | O(1) |
| `stop()` | Stop and reset | O(1) |
| `update()` / `update(now)` | Update state | O(1) |
| `tick()` / `tick(now)` | Check period completion | O(1) |
| `setPeriodMs(period)` | Change period | O(1) |
| `setDutyPerc(duty)` | Change duty cycle | O(1) |
| `isEnabled()` | Check if running | O(1) |
| `isOn()` | Check current phase | O(1) |

## Performance Characteristics

- **Memory usage**: ~25 bytes per instance
- **Execution time**: < 10µs per update() call (16MHz AVR)
- **Accuracy**: ±1ms (depends on `millis()` resolution)
- **Maximum period**: ~49 days (millis() rollover limit)

## Compatibility

| Platform | Compatibility | Notes |
|----------|---------------|-------|
| Arduino AVR (Uno, Nano) | Full support | Default millis() resolution |
| ESP8266 | Full support | Enhanced WiFi timing features |
| ESP32 | Full support | Optional microsecond precision |
| STM32 (Arduino Core) | Full support | 32-bit timing advantages |
| Raspberry Pi Pico | Full support | Dual-core considerations |

## Troubleshooting

### Common Issues

1. **Timer doesn't trigger**

   ```cpp
   // Ensure start() is called
   timer.start(); // ← Don't forget this!
   
   // Check period is reasonable
   timer.setPeriodMs(1000); // Not 0 or extremely large
   ```

2. **Duty cycle not changing**

   ```cpp
   // Clamping limits: 10% to 90%
   timer.setDutyPerc(5);  // Will become 10%
   timer.setDutyPerc(95); // Will become 90%
   ```

3. **Inaccurate timing in loop()**

   ```cpp
   void loop() {
       // Avoid blocking delays
       // timer.update(); // Good
       // delay(100);     // Bad - affects accuracy
   }
   ```

### Debugging Tips

```cpp
void debugTimer(ClockTimer &t) {
    Serial.print("Enabled: "); 
    Serial.print(t.isEnabled() ? "YES" : "NO");
    
    Serial.print(" | State: "); 
    Serial.print(t.isOn() ? "ON" : "OFF");
    
    Serial.print(" | Period: "); 
    Serial.print(t.getPeriodMs());
    Serial.print("ms");
    
    Serial.print(" | Duty: "); 
    Serial.print(t.getDutyPerc());
    Serial.print("%");
    
    Serial.print(" | ON Time: "); 
    Serial.print(t.getOnTime());
    Serial.print("ms");
    
    Serial.print(" | OFF Time: "); 
    Serial.print(t.getOffTime());
    Serial.println("ms");
}
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit changes (`git commit -am 'Add new feature'`)
4. Push to branch (`git push origin feature/improvement`)
5. Create Pull Request

## License

GNU General Public License v3.0 - see [LICENSE](LICENSE) file for details.

## Keywords & Tags

```text
#Arduino #Timer #PWM #Embedded #Timing #Clock #Microcontroller #ESP32 #AVR 
#STM32 #Library #Automation #Control #IoT #SignalGeneration #StateMachine 
#RealTime #Scheduling #Periodic #Events #Waveform #Precision #Lightweight
#OpenSource #GPLv3
```

## Support

- **Documentation**: [GitHub Docs](https://github.com/AlexZappy/ClockTimer/tree/main/docs)
- **Issues**: [GitHub Issues](https://github.com/AlexZappy/ClockTimer/issues)
- **Examples**: `/examples` folder in the library

## Related Projects

- [TimerOne](https://github.com/PaulStoffregen/TimerOne) - Hardware timer library
- [TaskScheduler](https://github.com/arkhipenko/TaskScheduler) - Cooperative multitasking
- [PWM](https://github.com/etherkit/PWM) - Advanced PWM control

---

**Quick Links**: [Examples](examples/) | [API Reference](docs/api_reference.md) | [Changelog](CHANGELOG.md) | [Contributing](CONTRIBUTING.md)

### Part of the Embedded Tools Collection by Alex Zappaterra
