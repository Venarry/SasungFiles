#include <WebServer.h>
WebServer server(80);

const String wifiName = "WindowHider";
const String wifiPassword = "PASSWORD";
const int ActivateTimeout = 5000;
int activeStateSignalPort = 13;
int deactiveStateSignalPort = 12;
bool isActive = false;
uint32_t lastActivateTime;

void setup() 
{
  pinMode(activeStateSignalPort, OUTPUT);
  StartServer();
}

void StartServer(){
  WiFi.softAP(wifiName, wifiPassword);
  //IPAddress myIP = WiFi.softAPIP();
  //WiFi.begin("Xiaomi_1EC2", "1q2w3e4r5t6y");

  server.on("/", ShowWebServer);

  server.on("/off", []() 
  {
    isActive = false;
    digitalWrite(activeStateSignalPort, LOW);
    ShowWebServer();
  });

  server.on("/on", []() 
  {
    isActive = true;
    digitalWrite(activeStateSignalPort, HIGH);
    ShowWebServer();
  });

  server.begin();
}

void loop() 
{
  server.handleClient();
}

void ShowWebServer() 
{
  String html = R"=====(
<!DOCTYPE html>
  <html>
  <head style="background-color: #cccccc; Color: blue;">
  <style>
  body {
    background: #f0d5d1;
    text-align: center;
    font-size: 32px;
  }

  p {
    font-size: 64px;
  }

  button {
    font-size: 64px;
  }
  </style>
  </head>

  <body>
        <h1>Web page for motion sensor control</h1>
  <hr size="#">

  <p>Sensor status:</p>
  )=====";

  if(isActive == true)
  {
    html += "<p>Active</p>";
  }
  else
  {
    html += "<p>Deactive</p>";
  }

  html += R"=====(

  <a href=\on><button>Sensor on</button></a>
  <a href=\off><button>Sensor off</button></a>

  </body>
  </html>
  )=====";

  server.send(200, "text/html", html);
}
