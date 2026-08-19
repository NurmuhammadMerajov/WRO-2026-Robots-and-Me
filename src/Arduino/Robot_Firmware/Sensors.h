#ifndef SENSORS_H
#define SENSORS_H

#include "Config.h"
#include <Wire.h>

struct SonarData {
    uint8_t leftDistance;
    uint8_t centerDistance;
    uint8_t rightDistance;
};

class Sensors {
public:
    void init();
    void update();
    
    SonarData getDistances();
    float getYawAngle();
    void resetYaw();

private:
    SonarData distances;
    float currentYaw;
    float gyroZBias;
    unsigned long lastTimeMicros;

    uint8_t readSonarCM(uint8_t trigPin, uint8_t echoPin);
    void calibrateGyro();
    float readRawGyroZ();
};

extern Sensors sensors;

#endif
