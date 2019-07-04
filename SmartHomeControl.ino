#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11 
#define LDRPIN A0// define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
const char* ssid = "HTC Portable Hotspot";
const char* password = "anuraj123";

#define ORG "tyczft"
#define DEVICE_TYPE "DURGA"
#define DEVICE_ID "123"
#define TOKEN "12345678"
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic1[] = "iot-2/cmd/home/fmt/String";
char topic2[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
void callback(char* topic1, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
  Serial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  pinMode(D1,OUTPUT);
  pinMode(D3,OUTPUT);
  wifiConnect();
  mqttConnect();
}

void loop(){
int sensorvalue =analogRead(A0);//a0
 Serial.println(sensorvalue);
float h = dht.readHumidity();
 float t = dht.readTemperature();

if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h,sensorvalue);
 if (!client.loop()) {
    mqttConnect();
  }
delay(100);
}



void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic1)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}
void PublishData(float temp, float humid,float light){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";
  payload += humid;
   payload+="," "\"light\":";
  payload += light;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic2, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
void callback(char* topic1, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic1);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "lighton"){
  digitalWrite(D1,HIGH);
  Serial.println("Light is Switched ON");
}
else if(command == "lightoff"){
  digitalWrite(D1,LOW);
  Serial.println("Light is Switched OFF");
}
else if(command == "fanon"){
  digitalWrite(D3,HIGH);
  Serial.println("fan is Switched ON");
}
else if(command == "fanoff"){
  digitalWrite(D3,LOW);
  Serial.println("fan is Switched OFF");
}
else if(command == "allon"){
  digitalWrite(D1,HIGH);
  digitalWrite(D3,HIGH);
  Serial.println("allon is Switched ON");
}
else if(command == "alloff"){
  digitalWrite(D1,LOW);
  digitalWrite(D3,LOW);
  Serial.println("allon is Switched OFF");
  }
command ="";
}
