// servo motor  = D1

// air quality sensor = A0

// temperature sensor = D2

// front ultrasonic sensor
// trig pin = D5
// echo pin = D6
const int FrontTrigPin = 14;
const int FrontEchoPin = 12;

// left ultrasonic sensor
// trig pin = D7
// echo pin = D0
const int LeftTrigPin = 13;
const int LeftEchoPin = 16;

// right ultrasonic sensor
// trig pin = D4
// echo pin = D8
const int RightTrigPin = 2;
const int RightEchoPin = 15;

// rear ultrasonic sensor
// trig pin = RX
// echo pin = TX
const int RearTrigPin = 3;
const int RearEchoPin = 1;



// defines variables
long Frontduration;
long Leftduration;
long Rightduration;
long Rearduration;

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
#include "DHT.h"

#define DHTPIN 4 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

float temp = 0.0;
float front = 0.0;
float left = 0.0;
float right = 0.0;
float rear = 0.0;

const char *ssid = "Smart Cradle";
const char *password = "admin123";

// Create AsyncWebServer object on port 80
AsyncWebServer server(8081);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>
    <h1>ok</h1>
</body>
</html>)rawliteral";

Servo myservo;  
int pos = 0;  
int MotorSpeed = 0;

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);

  dht.begin();
  pinMode(FrontTrigPin, OUTPUT); 
  pinMode(FrontEchoPin, INPUT); 
  pinMode(LeftTrigPin, OUTPUT); 
  pinMode(LeftEchoPin, INPUT); 
  pinMode(RightTrigPin, OUTPUT); 
  pinMode(RightEchoPin, INPUT); 
  pinMode(RearTrigPin, OUTPUT); 
  pinMode(RearEchoPin, INPUT); 

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  //myservo.attach(5);
  //myservo.write(pos);

  // Checking if reachable
  server.on("/check", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Crib Values of swinging
  server.on("/1", HTTP_GET, [](AsyncWebServerRequest *request) {
    MotorSpeed = 1;
    request->send_P(200, "text/html", index_html);
  });
  server.on("/2", HTTP_GET, [](AsyncWebServerRequest *request) {
    MotorSpeed = 2;
    request->send_P(200, "text/html", index_html);
  });
  server.on("/3", HTTP_GET, [](AsyncWebServerRequest *request) {
    MotorSpeed = 3;
    request->send_P(200, "text/html", index_html);
  });
  server.on("/0", HTTP_GET, [](AsyncWebServerRequest *request) {
    MotorSpeed = 0;
    request->send_P(200, "text/html", index_html);
  });
  
  //
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    float airqualityValue = analogRead(A0);
    float airquality = ((1023 - airqualityValue) / 1023) *100;

    String tempString = String(temp);
    String airQualityString = String(airquality);

    String frontString = String(front);
    String leftString = String(left);
    String rightString = String(right);
    String rearString = String(rear);

    request->send(200, "application/json", "{\"temperature\": \"" + tempString + "\", " + "\"airQuality\": \"" + airQualityString + "\", " + "\"front\": \"" + frontString + "\", " + "\"rear\": \"" + rearString + "\", " + "\"right\": \"" + rightString + "\", " + "\"left\": \"" + leftString + "\"}");
  });

  // Start server
  server.begin();
}

void loop() {
  TempSensor();
  FrontUltrasonicSensor();
  LeftUltrasonicSensor();
  RightUltrasonicSensor();
  RearUltrasonicSensor();
  ControlMotorSpeed();
}

void ControlMotorSpeed() {
  if (MotorSpeed == 0){
    myservo.detach();
    //myservo.write(0);
  }else if (MotorSpeed == 1){
    // for(pos = 0; pos <=180; pos +=5){
    //   myservo.write(pos);
    //   delay(50);
    // }
    // for(pos = 180; pos>=0; pos -=5){
    //   myservo.write(pos);
    //   delay(50);
    // }
    myservo.attach(5);
    myservo.write(100);
  }else if (MotorSpeed ==2){
    // for(pos = 0; pos <=180; pos +=5){
    //   myservo.write(pos);
    //   delay(25);
    // }
    // for(pos = 180; pos>=0; pos -=5){
    //   myservo.write(pos);
    //   delay(25);
    // }
    myservo.write(145);
  }else if (MotorSpeed ==3){
    // for(pos = 0; pos <=180; pos +=5){
    //   myservo.write(pos);
    // }
    // for(pos = 180; pos>=0; pos -=5){
    //   myservo.write(pos);
    // }
    myservo.write(180);
  }
}


void TempSensor() {
  temp = dht.readTemperature();
  if(isnan(temp)){
    temp = 0.0;
  }
}

void FrontUltrasonicSensor() {
  digitalWrite(FrontTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(FrontTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(FrontTrigPin, LOW);
  Frontduration = pulseIn(FrontEchoPin, HIGH);
  float frontdata = Frontduration * 0.0133 / 2;
  if (frontdata <= 30){
    front = frontdata;
  }else{
    front = 0;
  }
}


void LeftUltrasonicSensor() {
  digitalWrite(LeftTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(LeftTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(LeftTrigPin, LOW);
  Leftduration = pulseIn(LeftEchoPin, HIGH);
  float leftdata = Leftduration * 0.0133 / 2;
  if (leftdata <= 30){
    left = leftdata;
  }else{
    left = 0;
  }
}

void RightUltrasonicSensor() {
  digitalWrite(RightTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(RightTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(RightTrigPin, LOW);
  Rightduration = pulseIn(RightEchoPin, HIGH);
  float rightdata = Rightduration * 0.0133 / 2;
  if (rightdata <= 30){
    right = rightdata;
  }else{
    right = 0;
  }
}

void RearUltrasonicSensor() {
  digitalWrite(RearTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(RearTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(RearTrigPin, LOW);
  Rearduration = pulseIn(RearEchoPin, HIGH);
  float reardata = Rearduration * 0.0133 / 2;
  if (reardata <= 30){
    rear = reardata;
  }else{
    rear = 0;
  }
}
