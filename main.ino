#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Setor Tecnico"
#define STAPSK  "159conect357tec"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;
String responseJson;
int potenciometro;

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  potenciometro = analogRead(A0);
  Serial.println(potenciometro);

  responseJson = "{\"potenciometro\":\"";
  responseJson += potenciometro;
  responseJson += "\"}";

  server.send(200, "aplication/json", responseJson);

  digitalWrite(led, 0);

}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
