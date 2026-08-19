#include "Config.h"
#include "Actuators.h"
#include "Sensors.h"

unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 20; // 50 Hz telemetriya

void sendTelemetry() {
    SonarData sonars = sensors.getDistances();
    int16_t yawScaled = (int16_t)(sensors.getYawAngle() * 10.0f); // 0.1 daraja aniqlik

    uint8_t packet[7];
    packet[0] = TELEMETRY_START_BYTE;
    packet[1] = sonars.leftDistance;
    packet[2] = sonars.centerDistance;
    packet[3] = sonars.rightDistance;
    packet[4] = (uint8_t)(yawScaled >> 8);   // Yuqori bayt
    packet[5] = (uint8_t)(yawScaled & 0xFF); // Quyi bayt
    packet[6] = (packet[1] + packet[2] + packet[3] + packet[4] + packet[5]) & 0xFF; // Checksum

    Serial.write(packet, 7);
}

void processIncomingCommand() {
    // Raspberry Pi'dan paket formati: [0xAA] [STEER_OFFSET: int8_t] [PWM: int8_t] [MODE: uint8_t] [CHECKSUM]
    if (Serial.available() >= 5) {
        if (Serial.peek() == PACKET_START_BYTE) {
            Serial.read(); // Start byte ni o'qib tashlaymiz
            
            int8_t steerOffset = (int8_t)Serial.read();
            int8_t motorPwmRaw = (int8_t)Serial.read();
            uint8_t mode       = Serial.read();
            uint8_t checksum   = Serial.read();

            uint8_t calculatedChecksum = (steerOffset + motorPwmRaw + mode) & 0xFF;
            
            if (checksum == calculatedChecksum) {
                // int8_t (-128..127) ni to'liq PWM (-255..255) ga xaritaymiz
                int16_t fullPwm = ((int16_t)motorPwmRaw) * 2;
                
                actuators.setSteeringAngle(steerOffset);
                actuators.setMotorSpeed(fullPwm);

                if (mode == 0xFF) { // Reset Yaw buyrug'i
                    sensors.resetYaw();
                }
            }
        } else {
            Serial.read(); // Sinxronizatsiya buzilsa, keyingi baytga o'tish
        }
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    actuators.init();
    sensors.init();
}

void loop() {
    // 1. Datchiklarni yangilash (Yaw integratsiyasi va masofalar)
    sensors.update();

    // 2. Raspberry Pi buyruqlarini qabul qilish
    processIncomingCommand();

    // 3. 50 Hz tezlikda Raspberry Pi ga telemetriya yuborish
    if (millis() - lastTelemetryTime >= TELEMETRY_INTERVAL_MS) {
        lastTelemetryTime = millis();
        sendTelemetry();
    }
}
