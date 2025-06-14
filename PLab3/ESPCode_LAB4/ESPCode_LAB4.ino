#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include                "addons/RTDBHelper.h"
#include                "addons/TokenHelper.h"

#define   API_KEY       "REPLACE WITH YOURS" 
#define   DATABASE_URL  "REPLACE WITH YOURS"
#define   ssid          "REPLACE WITH YOURS"
#define   pass          "REPLACE WITH YOURS"

#define MOTOR1_EN         D1
#define MOTOR1_IN1        D2        
#define MOTOR1_IN2        D0

FirebaseData            fbdo;
FirebaseAuth            auth;
FirebaseConfig          config;

//SoftwareSerial mySerial(D6, D5);  // RX, TX for communication with Arduino

// VARIABLES
String Direction              ;
int    Speed                  ;
bool   signupok        = false;

float temp = 0.0, current = 0.0, voltage = 0.0;
int mySerialRecievedData = 0;

void setup() {
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR1_IN1,OUTPUT);
  pinMode(MOTOR1_IN2,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  Serial.begin(115200);
  //I2C
  Wire.begin(D3,D4);
  delay(100);
  Serial.print("Master Ready");
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to:");
  Serial.print(ssid); 
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url= DATABASE_URL;
  
  if(Firebase.signUp(&config,&auth,"",""))
  {
    Serial.println("OK");
    signupok=true;
  }
  else
  {
    Serial.println("Error");
  }

  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  Wire.beginTransmission(8); /* begin with device address 8 */
 Wire.write("Hello Arduino");  /* sends hello string */
 Wire.endTransmission();    /* stop transmitting */
  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
  // put your main code here, to run repeatedly:
  if(Firebase.ready() && signupok )
  {
    if(Firebase.RTDB.getInt(&fbdo, "/Speed"))
    {
      Speed = fbdo.intData();
      Serial.print("Speed: ");
      Serial.println(Speed);
    }
    if(Firebase.RTDB.getString(&fbdo, "/Direction"))
    {
      Direction = fbdo.stringData();
      Serial.print("Direction: ");
      Serial.println(Direction);
    }
    
    delay(100);
  }

  analogWrite(MOTOR1_EN,Speed);
  if(Direction == "CW")
  {
    digitalWrite(MOTOR1_IN2,LOW);
    digitalWrite(MOTOR1_IN1,HIGH);
  }
    else if(Direction == "CCW")
  {
    digitalWrite(MOTOR1_IN1,LOW);
    digitalWrite(MOTOR1_IN2,HIGH);
  }
  else
  {
    digitalWrite(MOTOR1_IN1,LOW);
    digitalWrite(MOTOR1_IN2,LOW);
  }
  
  while (Wire.available()) { // slave may send less than requested
    mySerialRecievedData = Wire.read();
    if(mySerialRecievedData == 1){
      temp = Wire.read();
      temp = temp/100;
    }

    if(mySerialRecievedData == 2){
      current = Wire.read();
      current = current/100;
    }

    if(mySerialRecievedData == 3){
      voltage = Wire.read();
      voltage = voltage/100;
    }
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Current: ");
    Serial.println(current);
    Serial.print("Voltage: ");
    Serial.println(voltage);
  }
  Firebase.RTDB.setFloat(&fbdo, "/Temp", temp);
  Firebase.RTDB.setFloat(&fbdo, "/Current", current);
  Firebase.RTDB.setFloat(&fbdo, "/Voltage", voltage);

  // If any threshold is exceeded, stop the motor
  
  analogWrite(MOTOR1_EN,Speed);  // Keep motor running
  Firebase.RTDB.setBool(&fbdo, "/motorStatus", true);
  
  

  delay(1000);  // Delay before reading data again
}