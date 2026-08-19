#include "Sensors.h"

Sensors sensors;

void Sensors::init() {
    Wire.begin();
    
    // Sonar pinlari
    pinMode(SONAR_LEFT_TRIG, OUTPUT);
    pinMode(SONAR_LEFT_ECHO, INPUT);
    pinMode(SONAR_CENTER_TRIG, OUTPUT);
    pinMode(SONAR_CENTER_ECHO, INPUT);
    pinMode(SONAR_RIGHT_TRIG, OUTPUT);
    pinMode(SONAR_RIGHT_ECHO, INPUT);

    // LSM6DSOX Giroskopni 104Hz / 2000 dps rejimida ishga tushirish
    Wire.beginTransmission(LSM6DSOX_I2C_ADDR);
    Wire.write(LSM6DSOX_CTRL2_G);
    Wire.write(0x4C); // 104 Hz ODR, 2000 dps
    Wire.endTransmission();

    currentYaw = 0.0f;
    lastTimeMicros = micros();
    
    calibrateGyro();
}

void Sensors::calibrateGyro() {
    float sum = 0.0f;
    const int samples = 150;
    
    for (int i = 0; i < samples; i++) {
        sum += readRawGyroZ();
        delay(5);
    }
    gyroZBias = sum / (float)samples;
}

float Sensors::readRawGyroZ() {
    Wire.beginTransmission(LSM6DSOX_I2C_ADDR);
    Wire.write(LSM6DSOX_OUTZ_L_G);
    Wire.endTransmission(false);
    Wire.requestFrom(LSM6DSOX_I2C_ADDR, 2);

    if (Wire.available() >= 2) {
        int16_t rawZ = Wire.read() | (Wire.read() << 8);
        // 2000 dps uchun sezgirlik: 70 mdps/LSB -> 0.07 dps/LSB
        return (float)rawZ * 0.070f;
    }
    return 0.0f;
}

uint8_t Sensors::readSonarCM(uint8_t trigPin, uint8_t echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, SONAR_TIMEOUT_US);
    if (duration == 0) return SONAR_MAX_DISTANCE_CM;
    
    uint8_t dist = duration / 58;
    return (dist > SONAR_MAX_DISTANCE_CM) ? SONAR_MAX_DISTANCE_CM : dist;
}

void Sensors::update() {
    // 1. Giroskop integratsiyasi (Yaw)
    unsigned long now = micros();
    float dt = (now - lastTimeMicros) / 1000000.0f;
    lastTimeMicros = now;

    float rawRateZ = readRawGyroZ() - gyroZBias;
    // Kichik shovqinlarni yo'qotish (Deadband)
    if (abs(rawRateZ) > 0.15f) {
        currentYaw += rawRateZ * dt;
    }

    // 2. Ultratovush datchiklarini ketma-ket o'qish (shovqin bermasligi uchun)
    distances.leftDistance   = readSonarCM(SONAR_LEFT_TRIG, SONAR_LEFT_ECHO);
    distances.centerDistance = readSonarCM(SONAR_CENTER_TRIG, SONAR_CENTER_ECHO);
    distances.rightDistance  = readSonarCM(SONAR_RIGHT_TRIG, SONAR_RIGHT_ECHO);
}

SonarData Sensors::getDistances() {
    return distances;
}

float Sensors::getYawAngle() {
    return currentYaw;
}

void Sensors::resetYaw() {
    currentYaw = 0.0f;
}
