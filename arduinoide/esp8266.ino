
You said:
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// ============= CẤU HÌNH NGƯỜI DÙNG =============
const char* ssid = "MANH QUAN";           // Thay bằng WiFi của bạn
const char* password = "0123456789";     // Thay bằng mật khẩu WiFi
#define BOT_TOKEN "8156778559:AAHMZ8iSFZUzrNCHR2J9v23ljJrtHibzQrA"    // Thay bằng Bot Token
#define CHAT_ID "7742254051"              // Thay bằng Chat ID
// ==============================================

// Cấu hình phần cứng
#define BUTTON_PIN D5    // GPIO5
#define GPS_RX_PIN D2    // GPIO4
#define GPS_TX_PIN D3    // GPIO0 (không sử dụng)

SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);
TinyGPSPlus gps;
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Biến toàn cục
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long WIFI_TIMEOUT = 10000; // 10 giây timeout WiFi
unsigned long lastTimeBotRan = 0;
const int botRequestDelay = 1000; // Kiểm tra tin nhắn mỗi 1 giây

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Kết nối WiFi
  connectWiFi();

  client.setInsecure(); // Bỏ qua xác thực SSL
  Serial.println("Hệ thống đã sẵn sàng!");
}

void loop() {
  // Xử lý nút nhấn
  handleButton();

  // Cập nhật dữ liệu GPS liên tục
  updateGPS();

  // Kiểm tra tin nhắn Telegram định kỳ
  if (millis() > lastTimeBotRan + botRequestDelay) {
    handleTelegramMessages();
    lastTimeBotRan = millis();
  }
}

void connectWiFi() {
  Serial.printf("\nĐang kết nối WiFi: %s", ssid);
  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nKết nối WiFi thành công!");
    Serial.print("Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nLỗi kết nối WiFi!");
    while (true) delay(1000); // Dừng chương trình
  }
}

void handleButton() {
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > 50) {
    if (reading == LOW) {
      sendAlert();
      delay(1000); // Chống nhiễu nút nhấn
    }
  }
  lastButtonState = reading;
}

void updateGPS() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayGPSInfo();
    }
  }
}

void sendAlert() {
  String message = "🆘 *TÔI CẦN TRỢ GIÚP KHẨN CẤP!* 🚨\n\n";

  if (gps.location.isValid() && gps.date.isValid()) {
    String googleMapsLink = "https://www.google.com/maps?q=" + 
                           String(gps.location.lat(), 6) + "," + 
                           String(gps.location.lng(), 6);
    
    message += "📍 *Tọa độ hiện tại:*\n";
    message += "┏ Vĩ độ: " + String(gps.location.lat(), 6) + "\n";
    message += "┣ Kinh độ: " + String(gps.location.lng(), 6) + "\n";
    message += "┣ Độ cao: " + String(gps.altitude.meters()) + "m\n";
    message += "┣ Số vệ tinh: " + String(gps.satellites.value()) + "\n";
    message += "┗ Độ chính xác: ±" + String(gps.hdop.value()/100.0, 2) + "m\n\n";
    message += "[🗺 Xem vị trí](" + googleMapsLink + ")";
  } else {
    message += "⚠️ *KHÔNG CÓ TÍN HIỆU GPS!*\n\n";
    message += "Thông tin bổ sung:\n";
    message += "┣ Thời gian khởi động: " + getUptime() + "\n";
    message += "┣ Cường độ WiFi: " + String(WiFi.RSSI()) + " dBm\n";
    message += "┗ Địa chỉ IP: " + WiFi.localIP().toString() + "";
  }

  if (bot.sendMessage(CHAT_ID, message, "Markdown")) {
    Serial.println("[Telegram] Đã gửi cảnh báo thành công!");
  } else {
    Serial.println("[Telegram] Gửi cảnh báo thất bại!");
  }
}

void displayGPSInfo() {
  Serial.print("Số vệ tinh: ");
  Serial.println(gps.satellites.value());
  Serial.print("Chất lượng tín hiệu: ");
  Serial.println(gps.hdop.value());
  Serial.print("Ngày/Giờ: ");
  if (gps.date.isValid()) {
    Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
                 gps.date.day(),
                 gps.date.month(),
                 gps.date.year(),
                 gps.time.hour(),
                 gps.time.minute(),
                 gps.time.second());
  }
  Serial.println("-----------------------");
}

String getUptime() {
  unsigned long seconds = millis() / 1000;
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = seconds % 60;
  return String(hours) + "h " + String(minutes) + "m " + String(secs) + "s";
}

// Hàm xử lý tin nhắn Telegram
void handleTelegramMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  for(int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (chat_id == CHAT_ID && text.equalsIgnoreCase("alo")) {
      sendAlert();
    }
  }
}