/*
  HTTP Advanced Authentication example
  Created Mar 16, 2017 by Ahmed El-Sharnoby.
  This example code is in the public domain.
*/

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "sunil";
const char* password = "8790225440";

WebServer server(80);

const char* www_username = "admin";
const char* www_password = "admin.123";
const char* www_realm = "Custom Auth Realm";
String authFailResponse = "Authentication Failed";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password))
    {
      return server.requestAuthentication(DIGEST_AUTH, www_realm, authFailResponse);
    }
    server.send(200, "text/plain", "Login success");
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
}

void loop()
{
  server.handleClient();
}
