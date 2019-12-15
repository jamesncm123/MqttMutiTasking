#include <PubSubClient.h>
#include <WiFi.h>
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// กำหนด wifi
const char* ssid = "junlada_2.4G"; // ชื่อ wifi
const char* password = "08081989"; // รหัส wifi
///////////////////////////////////////////
const char* mqtt_server = "192.168.1.111";
const int mqtt_port = 1883;
const char* MQTT_USER = "admin";
const char* MQTT_PASSWORD  = "admin";
char* mqtt_status_publish = "test";


TaskHandle_t t0;
TaskHandle_t t1;
TaskHandle_t t2;
int count = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//
 Serial.println();
    Serial.println();
    Serial.println();
    //// นับถอยหลัง 5 วินาที
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFi.begin(ssid, password);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    reconnect();
   //--------------------------------------------------//


 xTaskCreatePinnedToCore(
    tButtonFunc,
    "Button",
    10000,
    NULL,
    1,
    &t0,
    0);
    delay(500);
   xTaskCreatePinnedToCore(
    tLedFunc,
    "LED",
    10000,
    NULL,
    1,
    &t1,
    1);
    delay(500);
 xTaskCreatePinnedToCore(
    tMqttFunc,
    "MQTT",
    10000,
    NULL,
    2,
    &t2,
    0);
    delay(500);
//    delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
void tButtonFunc(void *params){
  pinMode(4,INPUT);
  
  while(true){
    int state = digitalRead(4);
    delay(10);
    if(state == 1){
      Serial.print("Release on core :");Serial.println(xPortGetCoreID());
      delay(1000);
    }else{
      Serial.print("Release on core :");Serial.println(xPortGetCoreID());
      count = count+1;
      Serial.print("count: ");Serial.println(count);
      delay(1000);
      if(count == 8){
        count = 0;
      }
    }
    
  }
}

void tLedFunc(void * params) {
  //set up
 // Serial.print("tLedFunc running on core ");Serial.println(xPortGetCoreID());
  
  
  // led.loop();
   pinMode(2,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  
//
//  //loop
  while (true) {
   // led.loop();
   if(count == 0){
    led(2,26,27,0,0,0);
    
   }
   if(count == 1){
    led(2,26,27,0,0,1);
   
   }
   if(count == 2){
    led(2,26,27,0,1,0);
   
   }
   if(count == 3){
    led(2,26,27,0,1,1);
   
   }
   if(count == 4){
    led(2,26,27,1,0,0);
    
   }
   if(count == 5){
    led(2,26,27,1,0,1);
   
   }
   if(count == 6){
    led(2,26,27,1,1,0);
   
   }
   if(count == 7){
    led(2,26,27,1,1,1);
    
   }
     Serial.print("tLedFunc running LOW on core ");Serial.println(xPortGetCoreID());
     
     delay(1000);
     
  }
}

void led(int pin1,int pin2,int pin3,bool statusLed1,bool statusLed2,bool statusLed3){
  digitalWrite(pin1,statusLed1);
  digitalWrite(pin2,statusLed2);
  digitalWrite(pin3,statusLed3);
  Serial.print(pin1);Serial.print(" : ");Serial.println(statusLed1);
  Serial.print(pin2);Serial.print(" : ");Serial.println(statusLed2);
  Serial.print(pin3);Serial.print(" : ");Serial.println(statusLed3);
  Serial.print("tLedFunc running on core ");Serial.println(xPortGetCoreID());
  sentMqtt("test",statusLed1,statusLed2,statusLed3);
}
void tMqttFunc(void *params){
  while(true){
    client.loop();
   delay(100);
  }
}
void sentMqtt(char* topic,int a,int b,int c){
  String msg = "";
  char chaMsg[20];
  msg += a;
  msg += b;
  msg += c;

  msg.toCharArray(chaMsg,20);
//  msg += (char)b;
//  msg += (char)c;
 Serial.print(topic); Serial.println(msg);
  client.publish(topic,chaMsg);
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    //topics = topic;
    String msg ="";
    Serial.print("data:");  
    Serial.write(payload, length);
    Serial.println();
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
 
}

void reconnect(){
  
   // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("test", "running");
      // ... and resubscribe
      client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  
}

//void tLedFunc2(void * params) {
//  //set up
// // Serial.print("tLedFunc running on core ");Serial.println(xPortGetCoreID());
//  pinMode(2,OUTPUT);
//  
//  // led.loop();
//   
//  
////
////  //loop
//  while (true) {
//    //led2.loop();
//     Serial.print("tLedFunc running HIGH on core ");Serial.println(xPortGetCoreID());
//     digitalWrite(2,HIGH);
//     delay(1000);
//     
//  }
//}
