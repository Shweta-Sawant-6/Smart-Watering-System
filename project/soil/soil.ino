
#include "DHT.h"
#include <ESP8266WiFi.h>

DHT dht;
const int sensor_pin = A0;  /* Connect Soil moisture analog sensor pin to A0 of NodeMCU */
int Pin_D4 = 2;
const char* ssid     = "iotserver1";
const char* password = "iotserver12";
const static int Dd1 = 5;

const char* host = "vishakatex.in";
void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
  dht.setup(Dd1);   /* D1 is used for data communication */
  pinMode(Pin_D4,OUTPUT);

  delay(10);
  Serial.println();
  Serial.print("connection to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WIFI Connected");
}

void loop() {
  float moisture_percentage;

  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
 
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  String mtr = "";
  if (moisture_percentage<=50)
   {
    digitalWrite(Pin_D4,HIGH);
     Serial.print("HIGH");
     mtr = "ON";
    delay(10);
    }
  if (moisture_percentage>50)
   {
    digitalWrite(Pin_D4,LOW);
    Serial.print("LOW");
    mtr = "OFF";
    delay(10);
    }
   
  delay(dht.getMinimumSamplingPeriod()); /* Delay of amount equal to sampling period */
  float humidity = dht.getHumidity();/* Get humidity value */
  float temperature = dht.getTemperature();/* Get temperature value */
  Serial.print(dht.getStatusString());/* Print status of communication */
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);/* Convert temperature to Fahrenheit units */
  delay(5000);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
  const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
  
  client.setInsecure(); 
  
  if (!client.connect(host, httpPort)) { //works!
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/agriculture/storedata.php?temp="+String(temperature)+"&soil="+String(moisture_percentage)+"&mtr="+String(mtr);
  


  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  Serial.println();
  delay(1000);
}
