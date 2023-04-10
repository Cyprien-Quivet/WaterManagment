#include <ESP8266WiFi.h>

const char* ssid = "Freebox-CM";
const char* password = "MarinaCyprien";
const char* ip = "192.168.1.10"; // adresse IP fixe de la carte
const int port = 80;

WiFiServer server(port);

int relayPin = D1;
int relayState = LOW;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayState);
  Serial.begin(115200);
  WiFi.config(IPAddress(192, 168, 1, 10), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }
  Serial.println("Connecté au WiFi !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("Client connecté !");
  while (client.connected()) {
    if (client.available() > 0) {
      int state = client.read() - '0';
      if (state == 1 || state == 0) {
        relayState = state;
        digitalWrite(relayPin, relayState);
        client.println(relayState);
        Serial.print("Relais : ");
        Serial.println(relayState);
      }
    }
  }
  delay(1);
  client.stop();
}
