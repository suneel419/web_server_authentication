#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char *ssid = "sunil";
const char *password = "8790225440";

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "hello from esp32!");
  });

  server.on(UriBraces("/users/{}"), []() {
  //server.on("/users/", []() {
    String user = server.pathArg(0);
    server.send(200, "text/plain", "User: '" + user + "'");
  });
  
  server.on(UriBraces("/users/{}/devices/{}"), []() {
  //  server.on("/users/{}/devices/{}", []() {

    String user = server.pathArg(0);
    String device = server.pathArg(1);
    server.send(200, "text/plain", "User: '" + user + "' and Device: '" + device + "'");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}



//use mdns library later for this code
