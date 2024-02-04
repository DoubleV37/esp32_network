#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define DHTPIN 10
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "wifi_name";
const char* password = "password";
const char* serverIP = "ip";  // Adresse IP du PC
const int serverPort = 12345;  // Port du serveur

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    // Serial.println("Connexion au WiFi en cours...");
  }
  Serial.println("Connecté au réseau WiFi");

  // Serial.print("Connexion au serveur ");
  // Serial.print(serverIP);
  // Serial.print(":");
  // Serial.println(serverPort);
  client.connect(serverIP, serverPort);
  // if (client.connect(serverIP, serverPort)) {
  //   Serial.println("Connexion établie");
  // } else {
  //   Serial.println("Échec de la connexion");
  // }
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // float hic = dht.computeHeatIndex(t, h, false);

  String message = "sonde1," + String(t, 2) + "," + String(h, 2);
  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C  Heat index: "));
  // Serial.println(hic);
  client.write(message.c_str(), message.length());
  delay(1800000);
}
