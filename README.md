# 🦯 GẬY THÔNG MINH HỖ TRỢ NGƯỜI KHIẾM THỊ & NGƯỜI GIÀ

## 🏆 THÀNH TÍCH NỔI BẬT
- 🥉 **Giải Ba – Cuộc thi Sáng tạo Thanh thiếu niên, Nhi đồng tỉnh Quảng Trị**
- 🥈 **Giải Nhì – Cấp Huyện**
- 🥈 **Giải Nhì – Cấp Trường**
- 📅 **Năm học: 2024 – 2025 (khi học Trung học phổ thông)**

---

## 📌 Giới thiệu dự án
Dự án **Gậy thông minh hỗ trợ người khiếm thị và người già yếu** được nghiên cứu và phát triển nhằm giúp người dùng **di chuyển an toàn – tự chủ – tiết kiệm chi phí**, thông qua việc tích hợp **đa cảm biến, GPS hai chiều và hệ thống cảnh báo linh hoạt**.

Theo thống kê:
- Việt Nam có hơn **2 triệu người khiếm thị**
- Khoảng **11% dân số trên 60 tuổi (~10 triệu người)** cần hỗ trợ di chuyển

Trong khi đó, các sản phẩm hiện nay:
- Giá thành cao  
- Chưa tích hợp đa cảm biến  
- Định vị còn hạn chế  

➡️ Dự án ra đời để giải quyết những vấn đề trên.

---

## 🎯 Mục tiêu dự án
- Nâng cao **an toàn khi di chuyển**
- Giúp người dùng **tự chủ hơn trong sinh hoạt**
- Giảm gánh nặng cho **gia đình và xã hội**
- Tạo ra sản phẩm **giá rẻ – dễ tiếp cận**

---

## ✨ Điểm nổi bật
- 🔹 **Đa cảm biến**: Siêu âm – Mưa – GPS
- 🔹 **Định vị GPS hai chiều qua Telegram**
- 🔹 **Cảnh báo đa kênh**: Loa + Tai nghe + Rung
- 🔹 **Pin sạc Type-C**, thời lượng dài
- 🔹 **Giá thành < 400.000 VNĐ**

---

## 🧠 Nguyên lý hoạt động
- **Cảm biến siêu âm (HC-SR04)**  
  - Phát hiện vật cản: 30 – 100 cm  
  - Phát hiện hố sâu: > 30 cm  

- **Cảm biến mưa**  
  - Phát hiện nước → cảnh báo trơn trượt  

- **GPS NEO-6M**  
  - Nhận tín hiệu vệ tinh  
  - Sai số < 3m  

- **ESP8266 + Telegram Bot**  
  - Gửi & nhận vị trí GPS hai chiều  
  - Người thân có thể theo dõi từ xa  

- **Hệ thống âm thanh & rung (DFPlayer)**  
  - Cảnh báo bằng giọng nói hoặc rung tùy môi trường  

---

## 🧩 Cấu hình phần cứng
| Thành phần | Mô tả |
|-----------|------|
| Arduino Uno | Xử lý trung tâm |
| ESP8266 | Kết nối Internet |
| HC-SR04 | Đo khoảng cách |
| Cảm biến mưa | Phát hiện trơn trượt |
| GPS NEO-6M | Định vị |
| DFPlayer Mini | Phát âm thanh |
| Pin + TP4056 | Sạc & quản lý pin |
| Khung gậy | Ống PVC + Foamex |

---

## 💻 Phần mềm sử dụng
- **Arduino IDE**
- **ESP8266 Arduino Core**
- **Telegram Bot API**
- **Ứng dụng Telegram (Android & iOS)**

---

## 🔬 Quy trình nghiên cứu
1. **Thiết kế phần cứng**
2. **Lập trình Arduino & ESP8266**
3. **Tích hợp Telegram Bot**
4. **Thử nghiệm thực tế**
5. **Hoàn thiện sản phẩm**

---

## 📊 Kết quả đạt được
- ✅ Độ chính xác cảnh báo > **90%**
- ✅ GPS sai số < **3m**
- ✅ Thời lượng pin: **8 – 10 giờ**
- ✅ Sạc đầy trong **2 – 3 giờ**
- ✅ Phản hồi vị trí **gần như tức thì**

---

## ⚠️ Hạn chế
- Cảm biến HC-SR04 chưa tối ưu trong môi trường quá ồn
- Chưa tích hợp AI nhận diện hình ảnh

---

## 🚀 Hướng phát triển
- 🔹 Thay HC-SR04 bằng **Lidar**
- 🔹 Tích hợp **Camera AI**
- 🔹 Pin dung lượng cao (21700) + sạc nhanh PD
- 🔹 Ứng dụng di động riêng (Bluetooth/4G)
- 🔹 Trợ lý giọng nói
- 🔹 Cảm biến nhịp tim & nút SOS

---

## 👨‍🎓 Tác giả 
- Võ Mạnh Quân
- Học sinh Trung học phổ thông  
- Năm học **2024 – 2025**
- Đề tài tham gia các cuộc thi **Sáng tạo trẻ**

---

## ❤️ Lời cảm ơn
Xin chân thành cảm ơn thầy cô, gia đình và bạn bè đã hỗ trợ, góp ý để dự án được hoàn thiện và đạt được các thành tích trên.
