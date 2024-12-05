#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// Servo configuration
#define MY_SERVO_PIN 13

// LED Pins
#define LED_RED 18
#define LED_GREEN 19
#define LED_PIN 2

// Pump configuration
#define PUMP_PIN 22  // Good - I/O capable
#define PUMP_FREQUENCY 5000
#define PUMP_CHANNEL 0
#define PUMP_RESOLUTION 8

// L298N Motor Driver pins and configuration
#define ENA 16  // Purple - Purple
#define ENB 17  // Brown - Green
#define IN1 27  // Grey - Grey
#define IN2 14  // Blue - Blue
#define IN3 12  // Green - Yellow
#define IN4 15  // Yellow - Blue
#define MOTOR_SPEED 255

// PWM Channels
#define MOTOR_A_CHANNEL 1  // Add PWM channel for ENA
#define MOTOR_B_CHANNEL 2  // Add PWM channel for ENB
#define MOTOR_FREQUENCY 5000
#define MOTOR_RESOLUTION 8

// Flame sensors (ADC1 pins - Input only)
#define FLAME_SENSOR_1 34
#define FLAME_SENSOR_2 35
#define FLAME_SENSOR_3 32

// ADC configuration
#define ADC_RESOLUTION 4095
#define MAPPED_MIN 0
#define MAPPED_MAX 100

// WebSocket
#define WEBSOCKET_PORT 81

#endif