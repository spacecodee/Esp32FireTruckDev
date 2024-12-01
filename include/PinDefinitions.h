#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// Servo configuration
#define MY_SERVO_PIN 13  // PWM capable

// LED Pins
#define LED_RED 18    // GPIO18
#define LED_GREEN 19  // GPIO19
#define LED_PIN 2     // Built-in LED

// Pump configuration
#define PUMP_PIN 22  // PWM capable
#define PUMP_FREQUENCY 5000
#define PUMP_CHANNEL 0
#define PUMP_RESOLUTION 8

// L298N Motor Driver pins
#define ENA 36  // Changed to ADC1_0
#define ENB 39  // Changed to ADC1_3
#define IN1 27  // Keep existing
#define IN2 14  // Keep existing
#define IN3 12  // Keep existing
#define IN4 15  // Keep existing
#define MOTOR_SPEED 200

// Flame sensors (ADC1 pins)
#define FLAME_SENSOR_1 34  // ADC1_6
#define FLAME_SENSOR_2 35  // ADC1_7
#define FLAME_SENSOR_3 32  // ADC1_4

// ADC configuration
#define ADC_RESOLUTION 4095  // Updated for ESP32 12-bit ADC
#define MAPPED_MIN 1
#define MAPPED_MAX 100

// WebSocket
#define WEBSOCKET_PORT 81

#endif