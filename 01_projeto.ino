#include <DHT.h>
#include <ESP8266WiFi.h>

const char *ssid =  "NET_2G3638F1";
const char *pass =  "653638F1";

const char* server = "api.thingspeak.com";
String apiKey = "IUORXK8YZ0GH2TSU";

String thingtweetAPIKey = "CEIONCYDFY1E1SS2";

WiFiClient client;

#define DHTPIN 5          //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);

int LED1 = 2;   //pin GPIO2
int LED2 = 16;  //PIN GPIO16

void setup() {  
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  delay(10);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);

  if (t > 25.0f) {
    updateTwitterStatus("Tweet enviado pela NodeMCU sobre a temperatura acima de 25ºC no Lab de Robótica");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  }

  if (t > 30.0f) {
    updateTwitterStatus("Tweet enviado pela NodeMCU sobre a temperatura acima de 25ºC no Lab de Robótica");
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  } 
  
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();
  Serial.println("Waiting...");
  
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
  delay(10000); 

}

void updateTwitterStatus(String tsData)
{
  if (client.connect(server, 80))
  {
    // Create HTTP POST Data
    tsData = "api_key=" + thingtweetAPIKey + "&status=" + tsData;
    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
  }
}
