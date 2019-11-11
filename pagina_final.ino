#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "DHT.h"
/*Put your SSID & Password*/
const char* ssid = "IMWICHO";  // Enter SSID here
const char* password = "wichocarrillo12";  //Enter Password here
#define DHTPIN D6 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int err;       
float temp, hum;
ESP8266WebServer server(80);

uint8_t LEDpin = 2;
bool LEDstatus = LOW;

void setup() {
  
  Serial.begin(9600);
  delay(100);
  pinMode(LEDpin, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  dht.begin();
  Serial.println();
}
void loop() {
  
  //Serial.println(hum);
  //Serial.println(temp);
  server.handleClient();
  if(LEDstatus)
  digitalWrite(LEDpin, HIGH);
  else
  digitalWrite(LEDpin, LOW);
}


void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led){
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.println(hum);
  Serial.println(temp);
 
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<head>\n";
  ptr +="<title>LED Y TEMPERATURA</title>\n";
  ptr += "<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css'>\n";
  ptr +="</head>\n";
  ptr +="<body style='background-color: #323232;'>\n";
  ptr +="<div class='card text-white bg-primary mb-3 mx-auto' style='max-width: 18rem;'>\n";
  ptr +="<div class='card-header' style='text-align: center;'>\n";
  ptr +="<h1>LED</h1>\n";
  ptr +="<form action='' method='GET'>\n";
  if(led)
    ptr +="<input type=\"button\" class='btn btn-dark' value=\"APAGAR LED\" onclick=\"window.location.href='/ledoff'\">\n";
  else
  ptr +="<input type=\"button\" class='btn btn-dark' value=\"ENCENDER LED\" onclick=\"window.location.href='/ledon'\">\n";
  ptr +="</form>\n";
  ptr +="<div class='card-body'>\n";
  if (!isnan(hum) || !isnan(temp)) {
  ptr +="<h5 class='card-title'>Temperatura</h5>\n";
  ptr +="<p class='card-text'>";
  ptr += temp;
  ptr += "</p>\n";
  ptr +="<h5 class='card-title'>Humedad</h5>\n";
  ptr +="<p class='card-text'>";
  ptr += hum;
  ptr += "</p>\n";}
  ptr +="</div>\n";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
