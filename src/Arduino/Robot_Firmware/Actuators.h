#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "Config.h"
#include <Servo.h>

class Actuators {
public:
    void init();
    void setSteeringAngle(int8_t targetAngleOffset); // -35° dan +35° gacha
    void setMotorSpeed(int16_t pwmSpeed);            // -255 dan +255 gacha
    void emergencyStop();

private:
    Servo steeringServo;
};

extern Actuators actuators;

#endif
