#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ================= UART PARAMETRLARI =================
#define SERIAL_BAUD         115200
#define PACKET_START_BYTE   0xAA
#define TELEMETRY_START_BYTE 0xBB

// ================= SERVO (RUL BOSHQARUVI) =================
#define SERVO_PIN           9
#define SERVO_CENTER_ANGLE  90
#define SERVO_MAX_LEFT      55
#define SERVO_MAX_RIGHT     125

// ================= TB6612FNG MOTOR DRAYVERI =================
#define MOTOR_PWMA          5    // PWM tezlik pini
#define MOTOR_AIN1          7    // Yo'nalish 1
#define MOTOR_AIN2          8    // Yo'nalish 2
#define MOTOR_STBY          6    // Standby (HIGH bo'lishi shart)

// ================= 3X ULTRATOVUSH (HC-SR04) =================
// Chap sensor
#define SONAR_LEFT_TRIG     12
#define SONAR_LEFT_ECHO     11

// Markaziy (old) sensor
#define SONAR_CENTER_TRIG   4
#define SONAR_CENTER_ECHO   3

// O'ng sensor
#define SONAR_RIGHT_TRIG    A0
#define SONAR_RIGHT_ECHO    A1

#define SONAR_MAX_DISTANCE_CM 200
#define SONAR_TIMEOUT_US     (SONAR_MAX_DISTANCE_CM * 58)

// ================= LSM6DSOX GIROSKOP (I2C) =================
#define LSM6DSOX_I2C_ADDR   0x6A // Yoki 0x6B
#define LSM6DSOX_CTRL2_G    0x11
#define LSM6DSOX_OUTZ_L_G   0x26

#endif
