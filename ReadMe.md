# Smart Medibox 🚨💊

![Medibox Simulation Part1](https://drive.google.com/file/d/11awVlbJ9S1ox487SCqrMmVtYjj-3p9oe/view?usp=drive_link)

![Medibox Simulation Part2](https://drive.google.com/file/d/15FLcp_ns4mahIzlJ8l4a0tSTJ3bicK8t/view?usp=drive_link)

An IoT-enabled smart medication box that:
1. Manages medication schedules with configurable alarms
2. Monitors ambient light conditions
3. Adjusts servo motor position via MQTT based on Node-RED logic
4. Allows remote parameter configuration through Node-RED dashboard

## Key Features ⚙️
- **Time Management**: Set medication times with timezone support
- **Alarm System**: Visual/audible alerts with snooze/disable functions
- **Light Sensing**: Calculates ambient light intensity
- **MQTT Integration**: Communicates with Node-RED backend
- **Servo Control**: Automatic lid opening based on conditions
- **Remote Configuration**: Adjust parameters via Node-RED dashboard

## Simulation Components 🖥️
- **Wokwi Simulation**: Primary development environment
- **Node-RED**: Logic processing and dashboard
- **MQTT Broker**: Real-time communication channel
- **ESP32**: Microcontroller emulation
- **Servo Motor**: Medication box lid actuator
- **LDR Sensor**: Light intensity measurement

## System Architecture
```mermaid
graph LR
    A[Wokwi Simulation] -->|MQTT| B(Node-RED)
    B -->|MQTT| A
    B --> C[(Database)]
    D[User] --> B
