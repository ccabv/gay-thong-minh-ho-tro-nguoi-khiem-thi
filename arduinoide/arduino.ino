#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// Cấu hình chân
#define TRIG_FRONT 6
#define ECHO_FRONT 5
#define TRIG_DOWN 9
#define ECHO_DOWN 8
#define RAIN_SENSOR A0
#define VIBRATION_PIN 2

// Cấu trúc điều khiển rung
struct VibrationProfile {
  unsigned long startTime;
  const uint16_t* pattern;
  uint8_t totalSteps;
  uint8_t currentStep;
  bool isActive;
};

// Trạng thái hệ thống
enum SystemState { 
  STATE_IDLE, 
  STATE_ALERT 
};

// Biến toàn cục
SystemState currentState = STATE_IDLE;
VibrationProfile vibraControl;
SoftwareSerial dfPlayerSerial(4, 3); // RX, TX
DFRobotDFPlayerMini dfPlayer;

// Mẫu rung chuẩn
const uint16_t VIB_30CM[] = {300, 0};        // Rung liên tục 300ms
const uint16_t VIB_50CM[] = {200, 100, 200, 0}; // 2 nhịp cách nhau 100ms
const uint16_t VIB_70CM[] = {100, 0};        // 1 nhịp 100ms
const uint16_t VIB_1M[] = {50, 0};           // Rung nhẹ 50ms

void setup() {
  // Thiết lập chân
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_DOWN, OUTPUT);
  pinMode(ECHO_DOWN, INPUT);
  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);

  // Khởi tạo Serial
  Serial.begin(9600);
  dfPlayerSerial.begin(9600);

  // Cấu hình DFPlayer
  if (!dfPlayer.begin(dfPlayerSerial)) {
    Serial.println("DFPlayer init error!");
    while (true);
  }
  
  dfPlayer.volume(30);
  dfPlayer.play(1); // Test phát âm thanh
  delay(1500);
}

void loop() {
  unsigned long currentMillis = millis();

  // Kiểm tra cảm biến khi hệ thống rảnh
  if (currentState == STATE_IDLE && (currentMillis - lastSensorCheck >= 100)) {
    checkSensors();
    lastSensorCheck = currentMillis;
  }

  // Xử lý rung
  if (vibraControl.isActive) {
    handleVibration(currentMillis);
  }
}

void checkSensors() {
  // Kiểm tra cảm biến khoảng cách dưới
  int distDown = getDistance(TRIG_DOWN, ECHO_DOWN);
  if (distDown > 27 || distDown == 0) {
    triggerAlert(5, 2000); // Cảnh báo rơi
    return;
  }

  // Kiểm tra cảm biến mưa
  int rainValue = analogRead(RAIN_SENSOR);
  if (rainValue < 500) {
    triggerAlert(6, 2000); // Cảnh báo mưa
    return;
  }

  // Kiểm tra cảm biến phía trước
  int distFront = getDistance(TRIG_FRONT, ECHO_FRONT);
  if (distFront > 0) {
    if (distFront < 30) {
      triggerAlert(2, 3000); // 30cm
    } else if (distFront < 50) {
      triggerAlert(3, 3000); // 50cm
    } else if (distFront < 70) {
      triggerAlert(4, 3000); // 70cm
    } else if (distFront < 100) {
      triggerAlert(7, 3000); // 1m
    }
  }
}

void triggerAlert(int trackNumber, unsigned long alertDuration) {
  currentState = STATE_ALERT;
  dfPlayer.play(trackNumber);
  
  // Thiết lập chế độ rung
  switch (trackNumber) {
    case 2: initVibration(VIB_30CM, sizeof(VIB_30CM)/sizeof(uint16_t)); break;
    case 3: initVibration(VIB_50CM, sizeof(VIB_50CM)/sizeof(uint16_t)); break;
    case 4: initVibration(VIB_70CM, sizeof(VIB_70CM)/sizeof(uint16_t)); break;
    case 7: initVibration(VIB_1M, sizeof(VIB_1M)/sizeof(uint16_t)); break;
    default: digitalWrite(VIBRATION_PIN, LOW); break;
  }
}

void initVibration(const uint16_t* pattern, uint8_t steps) {
  vibraControl.startTime = millis();
  vibraControl.pattern = pattern;
  vibraControl.totalSteps = steps;
  vibraControl.currentStep = 0;
  vibraControl.isActive = true;
  digitalWrite(VIBRATION_PIN, HIGH); // Bắt đầu với trạng thái HIGH
}

void handleVibration(unsigned long currentTime) {
  if (vibraControl.currentStep >= vibraControl.totalSteps) {
    vibraControl.isActive = false;
    digitalWrite(VIBRATION_PIN, LOW);
    return;
  }

  unsigned long elapsed = currentTime - vibraControl.startTime;
  uint32_t targetTime = 0;

  // Tính tổng thời gian đến bước hiện tại
  for (uint8_t i = 0; i <= vibraControl.currentStep; i++) {
    targetTime += vibraControl.pattern[i];
  }

  if (elapsed >= targetTime) {
    vibraControl.currentStep++;
    
    if (vibraControl.currentStep < vibraControl.totalSteps) {
      // Chuyển trạng thái rung (HIGH/LOW)
      bool state = (vibraControl.currentStep % 2 == 0);
      digitalWrite(VIBRATION_PIN, state ? HIGH : LOW);
    }
  }
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  return (duration > 0) ? (duration * 0.034) / 2 : 0;
}