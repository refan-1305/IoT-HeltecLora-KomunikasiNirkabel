// -------  Nama Program : Koneksi Nirkabel -------
//---- Author : Refan Rustoni Putra dan Muhammad Ibrahim Y -----
//------- Versi : 10  ------
//------- Ownership : Pribadi------ 
//------- Deskripsi : Belajar Mengenai TX, RX dan Cloud ------
//------- Mikro: Heltec Wifi LoRa 32(V3) ------------
//------- Pekan Ke 6 --------------

//============================================================
// RX LoRa Heltec V3 + ThingSpeak + Button Control
//============================================================

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <WiFi.h>
#include <HTTPClient.h>

// =========================
// WIFI
// =========================
const char* ssid = "mikro16";
const char* password = "mikro123";

// =========================
// THINGSPEAK
// =========================
String writeAPI = "QANHHHUP39Y10C7L";
String readAPI  = "QANHHHUP39Y10C7L";   // ganti dengan READ API key

// =========================
// PIN HELTEC V3
// =========================
#define LORA_NSS   8
#define LORA_SCK   9
#define LORA_MOSI 10
#define LORA_MISO 11
#define LORA_RST  12
#define LORA_BUSY 13
#define LORA_DIO1 14

SX1262 radio = new Module(LORA_NSS, LORA_DIO1, LORA_RST, LORA_BUSY);

WiFiClient client;

unsigned long lastCloud = 0;

void setup() {

  Serial.begin(115200);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);

  int state = radio.begin(921.0);

  if (state != RADIOLIB_ERR_NONE) {
    Serial.println("LoRa gagal start");
    while (true);
  }

  radio.setSpreadingFactor(7);
  radio.setBandwidth(125.0);
  radio.setCodingRate(5);
  radio.setSyncWord(0x12);

  Serial.println("RX READY");

  // WIFI CONNECT
  Serial.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop() {

  String data;

  int state = radio.receive(data);

  if (state == RADIOLIB_ERR_NONE) {

    Serial.println("\n=== DATA DITERIMA ===");
    Serial.println(data);

    float rssi = radio.getRSSI();
    float snr  = radio.getSNR();

    Serial.print("RSSI: ");
    Serial.println(rssi);

    Serial.print("SNR : ");
    Serial.println(snr);

    // =========================
    // PARSE DATA (suhu,hum)
    // =========================
    int koma = data.indexOf(',');

    float suhu = data.substring(0, koma).toFloat();
    float hum  = data.substring(koma + 1).toFloat();

    Serial.print("Suhu : ");
    Serial.println(suhu);

    Serial.print("Hum  : ");
    Serial.println(hum);

    // =========================
    // KIRIM KE THINGSPEAK
    // =========================
    if (millis() - lastCloud > 15000) {

      if (client.connect("api.thingspeak.com", 80)) {

        String url = "/update?api_key=" + writeAPI +
                     "&field1=" + String(suhu) +
                     "&field2=" + String(hum) +
                     "&field3=" + String(rssi) +
                     "&field4=" + String(snr);

        client.println("GET " + url + " HTTP/1.1");
        client.println("Host: api.thingspeak.com");
        client.println("Connection: close");
        client.println();

        Serial.println("Data dikirim ke ThingSpeak");

        lastCloud = millis();
      }

      client.stop();
    }

    // =========================
    // CEK BUTTON THINGSPEAK
    // =========================
    HTTPClient http;

    String readUrl =
    "http://api.thingspeak.com/channels/YOUR_CHANNEL_ID/fields/5/last.txt?api_key=" + readAPI;

    http.begin(readUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {

      String command = http.getString();

      command.trim();

      Serial.print("Button State: ");
      Serial.println(command);

      if (command == "1") {

        radio.transmit("ON");
        Serial.println("Perintah ON dikirim ke TX");
      }

      if (command == "0") {

        radio.transmit("OFF");
        Serial.println("Perintah OFF dikirim ke TX");
      }
    }

    http.end();
  }

  else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println("Receive error");
  }
}
