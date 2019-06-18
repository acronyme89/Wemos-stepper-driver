#include <ESP8266WiFi.h>    // Wifi on ESP8266
#include <PubSubClient.h>   // Mqtt library
#include <Stepper.h>        //Stepper Library

const int stepsPerRevolution = 32*64;
Stepper myStepper(stepsPerRevolution, D8, D7, D6, D5);
  
// Wifi acces - change value
const char* ssid = "SSID";
const char* password =  "WIFIPASSWD";

// MQTT Acces - change value
const char* mqttServer = "MQTT ADDRESS";
const int mqttPort = 1883;
const char* mqttUser = "YourMqttUser";
const char* mqttPassword = "YourMqttUserPassword";

// Initialize the client library
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  String mqttMessage;
  for (int i = 0; i < length; i++) {
   
    Serial.print((char)payload[i]);
    mqttMessage = mqttMessage +(char)payload[i];
  }
 
  Serial.println();
  Serial.println("-----------------------");
  delay(100);
  if (mqttMessage=="ON") {
    Serial.println("Reçu On");
    myStepper.setSpeed(10);
    int stepDone=0;
    while(stepDone<=204) {
      myStepper.step(10);
      stepDone++;
      Serial.println(stepDone);
      delay(5);
    }
  }
}
 
void loop() {
  client.loop();
}
