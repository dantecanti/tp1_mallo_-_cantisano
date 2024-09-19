#include <WiFi.h>
#include <WebServer.h>

const int relayPin = 2;  // Pin del relay
bool relayState = HIGH;   // Estado inicial del pin

// Configuración WiFi
const char* ssid = "IoTB";
const char* password = "inventaronelVAR";

// Crear un objeto de servidor web en el puerto 80
WebServer server(80);

// Función para cambiar el estado del relay
void toggleRelay() {
  relayState = !relayState;          // Cambiar estado
  digitalWrite(relayPin, relayState); // Aplicar estado al pin
  handleRoot();                      // Volver a la página principal
}

// Función para manejar la solicitud a la raíz ("/")
void handleRoot() {
  String html = "<!DOCTYPE html><html>";  // Asegúrate de que el tipo es 'String'
  html += "<head><meta http-equiv='refresh' content='10'/>";
  html += "<title>ESP32 Relay Control</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f0f8ff; text-align: center; padding: 50px;}";
  html += ".container { background-color: #ffffff; padding: 20px; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1); max-width: 400px; margin: auto;}";
  html += "h1 { color: #333333; }";
  html += "p { font-size: 1.5em; margin: 10px 0; }";
  html += "button { padding: 10px 20px; font-size: 1.2em; cursor: pointer; }";
  html += "</style></head>";

  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>Control de Relay</h1>";
  html += "<p>Estado actual del Relay: " + String(relayState ? "ENCENDIDO" : "APAGADO") + "</p>";
  html += "<form action='/toggle' method='POST'><button type='submit'>Cambiar Estado</button></form>";
  html += "</div>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);         // Configurar el pin del relay
  digitalWrite(relayPin, relayState); // Inicializar el pin en su estado inicial
  
  WiFi.begin(ssid, password);        // Conectar a la red WiFi
  Serial.print("Conectando a WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print("Aún conectando");
  }
  
  Serial.print("Conectado a WiFi. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/toggle", HTTP_POST, toggleRelay);  // Ruta para cambiar el estado del relay
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  // Manejar solicitudes HTTP
  server.handleClient();
}
