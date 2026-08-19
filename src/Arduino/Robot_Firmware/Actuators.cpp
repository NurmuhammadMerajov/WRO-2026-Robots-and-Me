#include "Actuators.h"

Actuators actuators;

void Actuators::init() {
    // Servoni biriktirish
    steeringServo.attach(SERVO_PIN);
    steeringServo.write(SERVO_CENTER_ANGLE);

    // Motor pinlarini sozlash
    pinMode(MOTOR_PWMA, OUTPUT);
    pinMode(MOTOR_AIN1, OUTPUT);
    pinMode(MOTOR_AIN2, OUTPUT);
    pinMode(MOTOR_STBY, OUTPUT);

    digitalWrite(MOTOR_STBY, HIGH); // Drayverni faollashtirish
    setMotorSpeed(0);
}

void Actuators::setSteeringAngle(int8_t targetAngleOffset) {
    int angle = SERVO_CENTER_ANGLE + targetAngleOffset;
    angle = constrain(angle, SERVO_MAX_LEFT, SERVO_MAX_RIGHT);
    steeringServo.write(angle);
}

void Actuators::setMotorSpeed(int16_t pwmSpeed) {
    pwmSpeed = constrain(pwmSpeed, -255, 255);

    if (pwmSpeed > 0) {
        // Oldinga
        digitalWrite(MOTOR_AIN1, HIGH);
        digitalWrite(MOTOR_AIN2, LOW);
        analogWrite(MOTOR_PWMA, pwmSpeed);
    } else if (pwmSpeed < 0) {
        // Orqaga
        digitalWrite(MOTOR_AIN1, LOW);
        digitalWrite(MOTOR_AIN2, HIGH);
        analogWrite(MOTOR_PWMA, abs(pwmSpeed));
    } else {
        // To'xtash (Brake)
        digitalWrite(MOTOR_AIN1, LOW);
        digitalWrite(MOTOR_AIN2, LOW);
        analogWrite(MOTOR_PWMA, 0);
    }
}

void Actuators::emergencyStop() {
    setMotorSpeed(0);
    setSteeringAngle(0);
}
