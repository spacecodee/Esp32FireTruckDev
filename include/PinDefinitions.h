#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// Servo configuration
#define MY_SERVO_PIN 13  // Good - I/O capable, ADC2_CH4

// LED Pins
#define LED_RED 18    // Good - I/O capable
#define LED_GREEN 19  // Good - I/O capable
#define LED_PIN 2     // Good - Built-in LED

// Pump configuration
#define PUMP_PIN 22  // Good - I/O capable
#define PUMP_FREQUENCY 5000
#define PUMP_CHANNEL 0
#define PUMP_RESOLUTION 8

// L298N Motor Driver pins
#define ENA 5   // Changed to GPIO5 - I/O capable
#define ENB 17  // Changed to GPIO17 - I/O capable
#define IN1 27  // Good - I/O capable
#define IN2 14  // Good - I/O capable
#define IN3 12  // Good - I/O capable
#define IN4 15  // Good - I/O capable
#define MOTOR_SPEED 200

// Flame sensors (ADC1 pins - Input only)
#define FLAME_SENSOR_1 34  // Good - Input only ADC1_CH6
#define FLAME_SENSOR_2 35  // Good - Input only ADC1_CH7
#define FLAME_SENSOR_3 32  // Good - I/O capable ADC1_CH4

// ADC configuration
#define ADC_RESOLUTION 4095  // Updated for ESP32 12-bit ADC
#define MAPPED_MIN 1
#define MAPPED_MAX 100

// WebSocket
#define WEBSOCKET_PORT 81

#endif