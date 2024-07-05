#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "00000000";
const char* password = "00000000";

AsyncWebServer server(80);

String getHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html lang=\"pt-BR\">";
  html += "<head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<title>Chamatron</title>";
  html += "<style>";
  html += "body { background-color: rgb(203, 219, 223); }";
  html += "div { display: flex; align-items: center; justify-content: center; }";
  html += ".button { border: none; color: white; padding: 16px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; font-weight: bold; margin: 4px 2px; transition-duration: 0.4s; cursor: pointer; border-radius: 10px; }";
  html += ".button1 { background-color: #04AA6D; color: black; }";
  html += ".button1:hover { background-color: #018152; color: white; }";
  html += ".button2 { background-color: red; color: black; }";
  html += ".button2:hover { background-color: #860202; color: white; }";
  html += ".button3 { background-color: #008CBA; color: black; }";
  html += ".button3:hover { background-color: #005774; color: white; }";
  html += "h1 { text-align: center; font-family: sans-serif; background-color: #005774; margin-top: 0px; padding: 10px; font-size: 50px; }";
  html += "table { font-family: arial, sans-serif; border-collapse: collapse; width: 70%; margin-top: 30px; }";
  html += "td { border: 2px solid #272727; text-align: left; padding: 8px; }";
  html += "th { border: 2px solid #272727; text-align: center; padding: 8px; }";
  html += "tr:nth-child(even) { background-color: #9c9c9c; }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<h1>CHAMATRON</h1>";
  html += "<div>";
  html += "<button class=\"button button1\" type=\"button\">LIGAR</button>";
  html += "<button class=\"button button2\" type=\"button\">DESLIGAR</button>";
  html += "<button class=\"button button3\" type=\"button\">CADASTRAR</button>";
  html += "</div>";
  html += "<div>";
  html += "<table>";
  html += "<tr><th colspan=\"2\">ALUNOS</th></tr>";
  html += "<tr><td>Nome</td><td>Matricula</td></tr>";
  html += "<tr><td>Nome</td><td>Matricula</td></tr>";
  html += "<tr><td>Nome</td><td>Matricula</td></tr>";
  html += "<tr><td>Nome</td><td>Matricula</td></tr>";
  html += "<tr><td>Nome</td><td>Matricula</td></tr>";
  html += "</table>";
  html += "</div>";
  html += "</body>";
  html += "</html>";

  return html;
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP8266's IP address
  Serial.print("ESP8266 Web Server's IP address: ");
  Serial.println(WiFi.localIP());

  // Home page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP8266 Web Server: New request received:");
    Serial.println("GET /");
    request->send(200, "text/html", getHTML());
  });

  // Route to control the car forward
  server.on("/car/forward", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("ESP8266 Web Server: New request received:");
    Serial.println("GET /car/forward");
    // Add the code to move the car forward
    request->send(200, "text/html", getHTML());
  });

  // Start the server
  server.begin();
}

void loop() {
  // Add your repeating code here, if any
}
