#include<WiFi.h>
#include<WiFiClient.h>
#include<WebServer.h>

const char *ssid="sunil";
const char *pass="8790225440";

WebServer server(80);

bool is_authentified()
{
  Serial.println("Enter is_authentified");
  if(server.hasHeader("cookie"))
  {
    Serial.println("Found cookie");
    String cookie=server.header("cookie");
    Serial.println(cookie);
    if(cookie.indexOf("ESPSESSIONID=1") != -1)
    {
      Serial.println("Authentification Succesfull");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

void handleLogin()
{
  String msg;
  if(server.hasHeader("Cookie"))
  {
    Serial.println("found Cookie");
    String cookie=server.header("Cookie");
    Serial.println(cookie);
  }

  if(server.hasArg("DISCONNECT"))
  {
    Serial.println("Disconnection");
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.sendHeader("Set-cookie","ESPSESSIONID=0");
    server.send(301);
    return;
  }

  if(server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
  {
    if(server.arg("USERNAME") == "sunil" && server.arg("PASSWORD") =="sunil.123")
    {
      server.sendHeader("LOCATION","/");
      server.sendHeader("cache-control","no-cache");
      server.sendHeader("set-cookie","ESPSESSIONID=1");
      server.send(301);
      Serial.println("Login Succesful");
      return;
    }
    msg="wrong username/password! Try Again";
    Serial.println("Login Failed");
  }

  String content = "<html><body  style='color:blue'><form action='/login' method='POST'>Tologin ,please use:sunil/sunil.123<br><br><br>";
  content += "User:<input type='text' name='USERNAME' placeholder='USERNAME'><br><br>";
  content += "Pass:<input type='password' name='PASSWORD' placeholder='password'><br><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>"+ msg +"<br>";
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200,"text/html",content);
}

void handleRoot()
{
  Serial.println("Enter handleRoot");
  String header;
  if(!is_authentified())
  {
    server.sendHeader("Location","/login");
    server.sendHeader("cache-control","no-cache");
    server.send(301);
    return;
  }
  String content="<body><H2>Hello,You succesfully connected to esp8266!</H2><br>";
  if(server.hasHeader("User-Agent"))
  {
    content += "The user agent used is:" + server.header("User-Agent") + "<br><br>";
  }

  content += "You can access This page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200,"text/html",content);
}

void handleNotFound()
{
  String message="File not Found";
  message += "URI:";
  message += server.uri();
  message += "\n Method:";
  message += (server.method() == HTTP_GET) ? "GET" :"POST";
  message += "\nArguments:";
  message += server.args();
  message += "\n";
  for(uint8_t i=0;i<server.args();i++)
  {
    message +=" "+ server.argName(i) + ":" + server.arg(i) + "\n";
     
  }
  server.send(404,"text/plain",message);
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
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

  server.on("/",handleRoot);
  server.on("/login",handleLogin);
  server.on("/inline",[]()
  {
    server.send(200,"text/plain","this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  const char * headerkeys[]={"User-Agent","Cookie"};
  size_t headerkeyssize = sizeof(headerkeys) /(sizeof(char *));
  server.collectHeaders(headerkeys,headerkeyssize);
  server.begin();
  Serial.println("Http Server started");
  
}



void loop() {
  // put your main code here, to run repeatedly:
server.handleClient();
}
