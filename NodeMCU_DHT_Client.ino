


/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTTYPE DHT22
const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);


byte ledPin = 2;
byte led = D0;
char ssid[] = "FLASH";           // SSID of your home WiFi
char pass[] = "BARRYALLEN";   // password of your home WiFi

char celsiusTemp[7];
char fahrenheitTemp[7];
char humidityTemp[7];

unsigned long askTimer = 0;

IPAddress server(192,168,43,13);       // the fix IP address of the server
WiFiClient client;

void setup() 
{
  dht.begin();
  Serial.begin(115200); // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
}

void loop () 
{
  client.connect(server, 80);   // Connection to the server
  while(client.connected())
  {
    String answer = client.readStringUntil('\r');
    if(answer.equals("")==false)
      Serial.println("Aye Aye Captain");
      
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      strcpy(celsiusTemp,"Failed");
      strcpy(fahrenheitTemp, "Failed");
      strcpy(humidityTemp, "Failed");
    }
    else
    {
      float hic = dht.computeHeatIndex(t, h, false);
      dtostrf(hic, 6, 2, celsiusTemp);
      float hif = dht.computeHeatIndex(f, h, true);
      dtostrf(hif, 6, 2, fahrenheitTemp);
      dtostrf(h, 6, 2, humidityTemp);
    }
    client.println(celsiusTemp);
    client.println(fahrenheitTemp);
    client.println(humidityTemp);      
  }
  client.flush();       
  
}
