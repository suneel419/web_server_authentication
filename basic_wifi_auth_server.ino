
#include<WiFi.h>
#include<WebServer.h>
const char * ssid="sunil";
const char * pass="8790225440";

const char* username="sunil";
const char* password="sunil.123";

WebServer server(80);

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
WiFi.mode(WIFI_STA);
WiFi.begin(ssid,pass);

while(WiFi.status() !=WL_CONNECTED)
{
  Serial.print(".");
}
Serial.println("Connected");

Serial.print(WiFi.localIP());

server.on("/",[](){
  if(!server.authenticate(username,password))
  {
   // return server.requestAuthentication();
  }
  server.send(200,"text/plain","Succesfull Login");
});
server.begin();
}

void loop() 
{
  server.handleClient();
}
