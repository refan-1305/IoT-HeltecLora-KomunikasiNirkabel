// -------  Nama Program : Koneksi Nirkabel -------
//---- Author : Refan Rustoni Putra dan Muhammad Ibrahim Y -----
//------- Versi : 10  ------
//------- Ownership : Pribadi------ 
//------- Deskripsi : Belajar Mengenai TX, RX dan Cloud ------
//------- Mikro: Heltec Wifi LoRa 32(V3) ------------
//------- Pekan Ke 5 --------------

// library
#include <WiFi.h>

// WIFI
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak
String apiKey = "0R8KHFVQ6SUM3WR5";
const char* server = "api.thingspeak.com";

WiFiClient client;

int counter = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

void loop() {

  // =========================
  // SIMULASI DATA LoRa RX
  // =========================
  counter++;

  String pesan = "Pesan dari Ibra ke Refan, Pesan ke-" + String(counter);

  float rssi = random(-90, -40);
  float snr  = random(5, 15);

  Serial.println(pesan);
  Serial.print("RSSI: "); Serial.println(rssi);
  Serial.print("SNR : "); Serial.println(snr);

  // =========================
  // KIRIM KE THINGSPEAK
  // =========================
  if (client.connect(server, 80)) {

    String url = "/update?api_key=" + apiKey +
                 "&field1=" + String(counter) +   // nomor pesan
                 "&field2=" + String(rssi) +      // RSSI
                 "&field3=" + String(snr);        // SNR

    Serial.println("Kirim ke ThingSpeak...");

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    // cek response
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Timeout!");
        client.stop();
        return;
      }
    }

    Serial.println("Response:");
    while (client.available()) {
      Serial.write(client.read());
    }

    Serial.println("\nData berhasil dikirim!\n");

  } else {
    Serial.println("Gagal koneksi ke ThingSpeak");
  }

  client.stop();

  delay(15000); // wajib ThingSpeak
}
