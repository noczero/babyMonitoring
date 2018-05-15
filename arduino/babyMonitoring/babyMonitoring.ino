/*
 Sensor
 vibrator D7 -- 13
 Voice A0
*/
// ESP Library
#include <ESP8266WiFi.h>
// MQTT Library
#include <PubSubClient.h>

#define vibrtPIN 13
#define voicePIN A0
#define LED1 14 //D5


// Wifi Connection

const char* wifiSSID = "eta terangkanlah";
const char* wifiPassword = "pamz8072";


// MQTT Define
 const char* mqttServerIP = "192.168.43.9";
//const char* mqttServerIP = "192.168.1.2";
const int mqttPort = 1883;

char* device1= "jarkomLAN/zeroDevice-1"; // pub & syv


WiFiClient myESP; // myESP become WIFI
PubSubClient client(myESP);

int timeDelay = 100;
bool statRunLED = false;
void wifiSetup(){
  WiFi.begin(wifiSSID,wifiPassword);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Waiting, connection to Wifi..");
    Serial.print("SSID : "); Serial.println(wifiSSID);
    
    // give notification LED
    // notifLED(LED2,200);
  }
  Serial.println("Connected to the WiFI Network "); 
  Serial.print("Connected Network "); Serial.println(wifiSSID);
  Serial.print("IP Local "); Serial.println(WiFi.localIP());
}


char dataPublish[50];
void publishMQTT(char* topics, String data){
   
   data.toCharArray(dataPublish, data.length() + 1);
    
   client.publish(topics, dataPublish);
}

void reconnect(){
  // MQTT Begin
  while(!client.connected()){
    Serial.println("Connecting to MQTT Server..");
    Serial.print("IP MQTT Server : "); Serial.println(mqttServerIP);
    //notifLED(LED3,300);
    bool hasConnection = client.connect("ZeroESP-1"); // connect(id,username,password) -> true if connect
    if(hasConnection){
      Serial.println("Success connected to MQTT Broker");
    } else {
      Serial.print("Failed connected");
      Serial.println(client.state());
      delay(2000);
      Serial.println("Try to connect...");
    }
  }
  client.publish(device1, "Reconnecting"); // acc
  client.subscribe(device1);  

}


void callback(char* topic, byte* payload, unsigned int length){
  Serial.println("--------");
  Serial.println("Message Arrived");
  Serial.print("Topic :"); Serial.println(topic);
  Serial.print("Message : ");
  String pesan = "";
  for(int i=0; i < length; i++){
    Serial.print((char)payload[i]);

    pesan += (char)payload[i];

  }
  Serial.println();

 // FOR TOPIC 1
 if (strcmp(topic,device1) == 0) {
   if(pesan == "true" ){
      Serial.println("LED 1 ON");
      digitalWrite(LED1, HIGH);

   } else if(pesan == "false"){
      Serial.println("LED 1 OFF");
      digitalWrite(LED1,LOW);
   }
   Serial.print("Masuk : " );
   Serial.println(pesan);
 } 
 
  Serial.print("Pesan masuk :");
  Serial.println(pesan);
  Serial.println("--------");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  wifiSetup();

  Serial.print(F("WiFi connected! IP address: "));
   //Initialize MQTT Connection
  client.setServer(mqttServerIP, mqttPort);
  client.setCallback(callback); // callback for incoming message

  // SETUP LED MODE
  pinMode(LED1, OUTPUT); // lampu

}

int voiceSensor = 0, vibratorSensor = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()){
      reconnect();
    }
    client.loop(); // looping forever the client
    

  //voiceSensor = analogRead(voicePIN);
  Serial.print("Sensor Suara : ");
  Serial.println(sensorSuara());

  //vibratorSensor = digitalRead(vibrtPIN);
  Serial.print("Sensor getar : ");
  Serial.println(sensorGetar());
  delay(100);
 
}

int sensorSuara(){
   voiceSensor = analogRead(voicePIN);


  return voiceSensor;
}

int sensorGetar(){
   vibratorSensor = digitalRead(vibrtPIN);
  
  return vibratorSensor;
}

