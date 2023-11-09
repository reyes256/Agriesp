#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

// WiFi
const char *ssid = "HACKATECNM7";
const char *password = "ITPUEBLA2023";

const int LED = 2;

WiFiClient espClient;
AsyncWebServer server(80);

void escribirArchivo(const char *ruta, const char *texto) {
  File archivo = LittleFS.open(ruta, "w");
  if (!archivo) {
    Serial.println("Archivo no se puede abir");
    return;
  }

  if (archivo.print(texto)) {
    Serial.println("Archivo Escribiendo");
  } else {
    Serial.println("Error Escribiendo");
  }
  archivo.close();
}

void leerArchivo(const char *ruta) {

  File archivo = LittleFS.open(ruta, "r");
  if (!archivo) {
    Serial.println("Archivo no se puede abir");
    return;
  }

  Serial.print("Contiene: ");
  while (archivo.available()) {
    Serial.write(archivo.read());
  }
  Serial.println();
  archivo.close();
}

void setup()
{
  pinMode(2, OUTPUT);
  Serial.begin(921600);
  Serial.println("\n\nIniciando de LittleFS");

  if (!LittleFS.begin(true)) {
    Serial.println("Error montando LittleFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando a Internet...");
  }
  Serial.println("Conexión a Internet Establecida");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED , HIGH);
    request->send(LittleFS, "/index.html");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED , LOW);
    request->send(LittleFS, "/index.html");
  });

  server.begin();
}

void reconnect()
{
  Serial.println("\nReintentando conexión WIFI...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Intentando reconexión a Internet...");
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Conexión a Internet Establecida");
    Serial.println(WiFi.localIP());
  }
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED){
    reconnect();
  }
  
  delay(2000);
}