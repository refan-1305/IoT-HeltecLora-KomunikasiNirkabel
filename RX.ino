// -------  Nama Program : Koneksi Nirkabel -------
//------- Author : Refan Rustoni Putra ------
//------- Versi : 10  ------
//------- Ownership : Pribadi------
//------- Deskripsi : Belajar Mengenai TX dan RX ------
//------- Mikro: Heltec Wifi LoRa 32(V3) ------------
//------- Pekan Ke 5 --------------

//------- Library ----------
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <RadioLib.h>

// =========================
// Heltec WiFi LoRa 32 V3
// SX1262 pin mapping
// =========================
#define LORA_NSS   8
#define LORA_SCK   9
#define LORA_MOSI 10
#define LORA_MISO 11
#define LORA_RST  12
#define LORA_BUSY 13
#define LORA_DIO1 14

#define OLED_SDA  17
#define OLED_SCL  18
#define OLED_RST  21

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,
  OLED_SCL,
  OLED_SDA,
  OLED_RST
);

// SX1262 module(cs, irq, rst, busy)
SX1262 radio = new Module(LORA_NSS, LORA_DIO1, LORA_RST, LORA_BUSY);

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, 12, "Heltec V3 SX1262 RX");
  u8g2.sendBuffer();

  int state = radio.begin(923.0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Radio init failed, code: ");
    Serial.println(state);

    u8g2.clearBuffer();
    u8g2.drawStr(0, 12, "Radio init FAILED");
    u8g2.sendBuffer();

    while (true) {
      delay(1000);
    }
  }

  radio.setSpreadingFactor(7);
  radio.setBandwidth(125.0);
  radio.setCodingRate(5);
  radio.setSyncWord(0x12);

  Serial.println("RX ready");
}

void loop() {
  String str;
  int state = radio.receive(str);

  if (state == RADIOLIB_ERR_NONE) {
    float rssi = radio.getRSSI();
    float snr  = radio.getSNR();

    Serial.println("Received packet:");
    Serial.println(str);
    Serial.print("RSSI: ");
    Serial.println(rssi);
    Serial.print("SNR : ");
    Serial.println(snr);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(0, 12, "Heltec V3 SX1262 RX");
    u8g2.drawStr(0, 28, "Data:");
    u8g2.drawStr(0, 44, str.c_str());

    char info[32];
    snprintf(info, sizeof(info), "RSSI: %.1f", rssi);
    u8g2.drawStr(0, 60, info);
    u8g2.sendBuffer();
  }
  else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("Receive failed, code: ");
    Serial.println(state);
  }
}
