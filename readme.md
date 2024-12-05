# ESP32 Fire Truck Control System

A remote-controlled fire truck system using ESP32 microcontroller with WebSocket communication and a web-based control
interface.

## Features

### Hardware Control

- LED Control System
    - Red LED control (GPIO18)
    - Green LED control (GPIO19)
    - Built-in LED status (GPIO2)

- Flame Detection System
    - 3 flame sensors (GPIO34, 35, 32)
    - Real-time flame intensity monitoring
    - 1-100% intensity mapping

- Motor Control System
    - L298N motor driver integration
    - 4-wheel drive control
    - Directional movements:
        - Forward/Backward
        - Left/Right turns
        - Diagonal movements (Forward-Left, Forward-Right, etc)
    - Speed control via PWM
    - Safety stop on WiFi disconnect

- Water Pump System
    - PWM-controlled water pump
    - ON/OFF control
    - Safety shutoff features

### Motor Control

- Dual motor control system
    - Speed control for Motor A
    - Speed control for Motor B
    - Direction control
- Real-time motor status feedback

### Connectivity Features

- WiFi Management
    - Automatic WiFi portal configuration
    - Remote WiFi reset capability
    - Connection status monitoring
    - Real-time status updates

- WebSocket Communication
    - Real-time bidirectional control
    - JSON-based command structure
    - Status feedback system
    - Connection health monitoring

### Connectivity

- WiFi connectivity using WiFiManager
- WebSocket server for real-time communication
- Web-based control interface

### Pin Configuration
All pin definitions are centralized in `include/PinDefinitions.h`:
- Motor control pins (ENA, ENB, IN1-IN4)
- LED indicators
- Flame sensors
- Water pump control

### WebSocket Commands
The system accepts JSON commands for:
- Motor control
- LED control
- Pump control
- WiFi reset
- Sensor data requests

## Technologies Used

### Hardware

- ESP32 Development Board
- Motor control system
- LED indicators
- Servo motors

### Software

- PlatformIO for ESP32 development
- Arduino framework
- Libraries:
    - WiFiManager (v2.0.17)
    - ESP32Servo (v3.0.5)
    - WebSockets (v2.6.1)
    - ArduinoJson (v7.2.1)

## Project Structure

├── include/

│ ├── PinDefinitions.h # Hardware pin configurations

│ ├── WebSocketConfig.h # WebSocket server
configuration

│ └── WifiConfig.h # WiFi connection settings

├── src/

│ ├── main.cpp # Main application code

│ ├── WebSocketConfig.cpp # WebSocket implementation

│ └── WifiConfig.cpp # WiFi configuration implementation

├── platformio.ini # PlatformIO configuration

└── esp32.md # ESP32 pin mapping documentation

## Building and Flashing

1. Install PlatformIO IDE
2. Clone this repository
3. Open project in PlatformIO
4. Build the project:

```bash
pio run
```

## Upload to ESP32:

```bash
pio run -t upload
```

Configuration
-------------

The system can be configured through:

- WiFiManager portal for network settings
- WebSocket interface for real-time control
- Pin definitions in the include files

Contributing
------------

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Submit a pull request

License
-------

This project is open source and available under the MIT [License](LICENSE).