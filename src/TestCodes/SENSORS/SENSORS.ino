#include <Servo.h>
#include <Wire.h>

// ==================== PINLAR VA SOZLAMALAR ====================
#define buttonPin 6
#define servoPin 9

// Sizning oxirgi sozlamangizdagi pinlar:
#define motorPWM 5
#define motorForward 7
#define motorBackward 4

#define SERVO_LEFT 57
#define SERVO_FORWARD 89 
#define SERVO_RIGHT 113

// ==================== GIROSKOP (LSM6DSOX) ====================
#define LSM6DSOX_ADDR 0x6A
#define CTRL2_G       0x11
#define OUTZ_L_G      0x26

Servo steering;

bool started = false;
bool sequenceFinished = false; 

const int slowSpeed = 120; // Tezlikni robotning og'irligiga qarab sozlaysiz (0-255)

// Giroskop o'zgaruvchilari
float gyroZ_bias = 0.0;
float current_yaw = 0.0;
unsigned long prev_time = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(motorPWM, OUTPUT);
  pinMode(motorForward, OUTPUT);
  pinMode(motorBackward, OUTPUT);
  
  steering.attach(servoPin);
  steering.write(SERVO_FORWARD);

  // --- GIROSKOPNI KALIBRLASH ---
  Serial.println("Giroskop kalibrlanmoqda, tekis tursin...");
  Wire.beginTransmission(LSM6DSOX_ADDR);
  Wire.write(CTRL2_G);
  Wire.write(0x44); 
  Wire.endTransmission();
  delay(500);

  long z_sum = 0;
  int samples = 500;
  for (int i = 0; i < samples; i++) {
    z_sum += readRawGyroZ();
    delay(4);
  }
  gyroZ_bias = (z_sum / (float)samples) * 0.0175;
  prev_time = micros();

  Serial.println("Tayyor! Boshlash uchun tugmani bosing.");
}

void loop() {
  updateGyro();

  if (digitalRead(buttonPin) == LOW && !sequenceFinished) {
    delay(200); // Debounce
    started = true;
    Serial.println("--- 3 TA TO'LIQ KRUG BOSHLANDI ---");
  }

  if (started && !sequenceFinished) {
    
    // 4 ta burilish uchun kerakli burchaklar ketma-ketligi
    float turnAngles[4] = {80.0, 80.0, 80.0, 80.0};
    
    // Yurish vaqtlari: birinchi tomon 3400, qolgan uchtasi 3600
    unsigned long driveDurations[4] = {3400, 3600, 3600, 3600};

    // 3 marta to'liq krug aylanishi uchun sikl
    for (int krug = 1; krug <= 3; krug++) {
      Serial.print(krug); Serial.println(" - krug boshlandi");

      // Har bir krugda 4 ta tomon va 4 ta xil burilish bor
      for (int tomon = 1; tomon <= 4; tomon++) {
        Serial.print("Tomon: "); Serial.println(tomon);
        Serial.print("Yurish vaqti (ms): "); Serial.println(driveDurations[tomon - 1]);
        Serial.print("Burilish burchagi: "); Serial.println(turnAngles[tomon - 1]);
        
        // 1. Massivdagi mos vaqt bo'yicha oldinga yurish
        driveStraight(driveDurations[tomon - 1]); 

        // 2. Massivdagi mos gradus bo'yicha chapga burilish
        turnLeftGyro(turnAngles[tomon - 1]);
      }
      
      Serial.print(krug); Serial.println(" - krug tugadi. 1 soniya tanaffus...");
      delay(1000); // Har bir krugdan keyin qisqa pauza
    }

    started = false;
    sequenceFinished = true; // Qayta ishlamasligi uchun
    Serial.println("--- BARCHA HARAKATLAR YAKUNLANDI ---");
  }
}

// ==================== YORDAMCHI FUNKSIYALAR ====================

// Oldinga yurish funksiyasi
void driveStraight(unsigned long durationMs) {
  steering.write(SERVO_FORWARD);
  motorControl(slowSpeed);
  unsigned long startMillis = millis();
  
  while (millis() - startMillis < durationMs) {
    updateGyro(); // Vaqt o'tishi bilan giroskopni ham o'qib boramiz
    delay(2);
  }
  
  // Masofa tugagach motorni to'xtatish
  motorControl(0);
  delay(300);
}

// Giroskop bilan chapga aniq berilgan burchakka burilish
void turnLeftGyro(float targetAngle) {
  float startYaw = current_yaw; 
  
  steering.write(SERVO_LEFT); // Rul chapga buriladi
  motorControl(slowSpeed); 

  while (abs(current_yaw - startYaw) < targetAngle) {
    updateGyro();
    delay(2);
  }

  // Burchakka yetgach servoni tekislaymiz va motorni to'xtatamiz
  steering.write(SERVO_FORWARD);
  motorControl(0); 
  delay(300); // Muvozanat uchun pauza
}

// Motorni boshqarish 
void motorControl(int speed) {
  if (speed > 0) {
    digitalWrite(motorForward, HIGH);
    digitalWrite(motorBackward, LOW);
    analogWrite(motorPWM, speed);
  } else if (speed < 0) {
    digitalWrite(motorForward, LOW);
    digitalWrite(motorBackward, HIGH);
    analogWrite(motorPWM, abs(speed));
  } else {
    digitalWrite(motorForward, LOW);
    digitalWrite(motorBackward, LOW);
    analogWrite(motorPWM, 0);
  }
}

// Giroskopni yangilash
void updateGyro() {
  unsigned long curr_time = micros();
  float dt = (curr_time - prev_time) / 1000000.0; 
  prev_time = curr_time;
  float rate_z = readRawGyroZ() * 0.0175;
  current_yaw += (rate_z - gyroZ_bias) * dt;
}

// Giroskopdan xom ma'lumotni o'qish
int16_t readRawGyroZ() {
  Wire.beginTransmission(LSM6DSOX_ADDR);
  Wire.write(OUTZ_L_G);
  Wire.endTransmission(false);
  
  Wire.requestFrom(LSM6DSOX_ADDR, 2);
  if (Wire.available() == 2) {
    uint8_t z_l = Wire.read();
    uint8_t z_h = Wire.read();
    return (int16_t)(z_h << 8 | z_l); 
  }
  return 0;
}
