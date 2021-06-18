/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
char ssid[] = "**********";               // SSID of your home WiFi
char pass[] = "**********";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 43, 11);            // IP address of the server
IPAddress gateway(192,168,43,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

String hum="";
String tempC="";
String tempF="";

void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
}

void loop () 
{
  WiFiClient client = server.available();
  if (client) 
  {
    if (client.connected()) 
    {
      Serial.println(".");
      client.println("Keep updating the data");
      hum= client.readStringUntil('\r');    // receives the humidity from the client
      tempC= client.readStringUntil('\r');  // receives the temperature in C the client
      tempF= client.readStringUntil('\r');  // receives the temperature in F from the client
      client.flush();

      Serial.println("Humidity : "); Serial.println(hum); Serial.println("%");
      Serial.println("Temperature : "); Serial.println(tempC); Serial.println("º");
      Serial.println("Temperature : "); Serial.println(tempF); Serial.println("º");
    }
    client.stop();
  }
}
