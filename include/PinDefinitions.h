#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// LED Pins
#define LED_RED 18
#define LED_GREEN 19
#define LED_PIN 2

// Servo configuration
#define SERVO_PIN 13
#define MIN_ANGLE 30
#define MAX_ANGLE 130
#define INITIAL_POSITION 0
#define SERVO_DELAY 15

// Pump configuration
#define PUMP_PIN 22
#define PUMP_FREQUENCY 5000
#define PUMP_CHANNEL 0
#define PUMP_RESOLUTION 8

// L298N Motor Driver pins
#define ENA 25
#define ENB 26
#define IN1 27
#define IN2 14
#define IN3 12
#define IN4 33
#define MOTOR_SPEED 255

// Flame sensors
#define FLAME_SENSOR_1 34
#define FLAME_SENSOR_2 35
#define FLAME_SENSOR_3 32

// ADC configuration
#define ADC_RESOLUTION 4095
#define MAPPED_MIN 1
#define MAPPED_MAX 100

// WebSocket
#define WEBSOCKET_PORT 81

#endif  // PIN_DEFINITIONS_H