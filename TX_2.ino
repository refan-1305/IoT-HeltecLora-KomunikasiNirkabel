//---- information about the code ----
// project: Teknologi IoT Pekan 6
// created by: Muhammad Ibrahim Yusuf (10824006)(Tx)
// project partner: Refan Rustoni Putra (10824005)(Rx)
// deskripsi: LoRa TX Heltec V3 + pembacaan sensor DHT(dummy)

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

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

#define DEVICE_PIN 15

SX1262 radio = new Module(LORA_NSS, LORA_DIO1, LORA_RST, LORA_BUSY);

// =========================
// TIMER
// =========================
unsigned long lastSend = 0;
const unsigned long sendInterval = 15000;

void setup() {
  Serial.begin(115200);

  pinMode(DEVICE_PIN, OUTPUT);
  digitalWrite(DEVICE_PIN, LOW);

  randomSeed(analogRead(0));

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_NSS);

  if (radio.begin(921.0) != RADIOLIB_ERR_NONE) {
    Serial.println("LoRa init failed");
    while (true);
  }

  radio.setOutputPower(14);
  radio.setSpreadingFactor(7);
  radio.setBandwidth(125.0);
  radio.setCodingRate(5);
  radio.setSyncWord(0x12);

  radio.startReceive();

  Serial.println("=== TX READY ===\n");
}

void loop() {

  // =========================
  // KIRIM DATA PERIODIK
  // =========================
  if (millis() - lastSend >= sendInterval) {

    lastSend = millis();

    float suhu = random(200, 350) / 10.0;
    float hum  = random(400, 900) / 10.0;

    String payload = String(suhu, 1) + "," + String(hum, 1);

    radio.standby();
    delay(50);

    int state = radio.transmit(payload);

    // =========================
    // TAMPILAN SERIAL RAPI
    // =========================
    Serial.println("===== DATA TERKIRIM =====");
    Serial.println("Data Terkirim dari Ibra ke Refan");

    Serial.print("Suhu       : ");
    Serial.print(suhu, 2);
    Serial.println(" °C");

    Serial.print("Kelembapan : ");
    Serial.print(hum, 2);
    Serial.println(" %");

    Serial.print("Payload    : ");
    Serial.println(payload);

    Serial.print("Status     : ");
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println("BERHASIL");
    } else {
      Serial.println("GAGAL");
    }

    Serial.println("========================\n");

    delay(100);
    radio.startReceive();
  }

  // =========================
  // TERIMA COMMAND
  // =========================
  String incoming;

  int state = radio.receive(incoming);

  if (state == RADIOLIB_ERR_NONE) {

    Serial.println("<<< COMMAND MASUK >>>");
    Serial.print("Command    : ");
    Serial.println(incoming);

    if (incoming == "ON") {
      digitalWrite(DEVICE_PIN, HIGH);
      Serial.println("Device     : MENYALA");
    } 
    else if (incoming == "OFF") {
      digitalWrite(DEVICE_PIN, LOW);
      Serial.println("Device     : MATI");
    }

    Serial.println("======================\n");

    radio.startReceive();
  }
}
