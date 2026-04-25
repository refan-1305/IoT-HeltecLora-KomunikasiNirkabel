# LoRa Communication ESP32 Heltec V3 (TX - RX)

## Deskripsi
Project ini merupakan implementasi komunikasi data menggunakan teknologi LoRa (Long Range) dengan memanfaatkan dua buah board ESP32 Heltec LoRa V3 sebagai transmitter (TX) dan receiver (RX).

Data dikirim dari TX ke RX secara wireless menggunakan modul LoRa SX1262, kemudian ditampilkan pada Serial Monitor.

## Fitur
- Komunikasi LoRa (TX & RX)
- Pengiriman data secara wireless
- Monitoring data melalui Serial Monitor
- Monitoring kualitas sinyal (RSSI & SNR)

## Hardware yang Digunakan
- 2x ESP32 Heltec LoRa V3 (WiFi LoRa 32 V3)
- Kabel USB

## Software & Library
- Arduino IDE
- RadioLib
- U8g2 (OLED)
- SPI.h
- Wire.h

## Cara Kerja Sistem
1. ESP32 TX membuat data berupa pesan teks
2. Data dikirim melalui modul LoRa (SX1262)
3. Sinyal dikirim secara wireless menggunakan frekuensi 923 MHz
4. ESP32 RX menerima data dari LoRa
5. Data ditampilkan pada Serial Monitor 
6. Sistem menampilkan nilai RSSI dan SNR sebagai indikator kualitas sinyal

## Alur Sistem
TX → LoRa → (Wireless) → LoRa → RX → Serial Monitor

## Hasil
- Data berhasil dikirim dari TX ke RX
- Data berhasil diterima dan ditampilkan
- Sistem mampu menampilkan RSSI dan SNR
- Komunikasi berjalan stabil pada kondisi normal

## Catatan
- Pastikan frekuensi TX dan RX sama (contoh: 923 MHz)
- Pastikan parameter LoRa (Spreading Factor, Bandwidth, Coding Rate) sama
- Gunakan jarak yang sesuai untuk menghindari gangguan sinyal
