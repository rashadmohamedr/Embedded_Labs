#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#include                "addons/RTDBHelper.h"
#include                "addons/TokenHelper.h"

#define   API_KEY       "AIzaSyAQHiZpuRo2Y5p5A6tLWHkfmT3Om9moH4Y"
#define   DATABASE_URL  "https://cie405-lab1-55ebf.firebaseio.com/"
#define   ssid          "Ibrahem's Galaxy Note20 Ultra"
#define   pass          "19031983"

#define MOTOR1_EN         D5
#define MOTOR1_IN1        D6        
#define MOTOR1_IN2        D7

FirebaseData            fbdo;
FirebaseAuth            auth;
FirebaseConfig          config;

String Direction              ;
int    Speed                  ;
bool   signupok        = false;
void setup() {

  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR1_IN1,OUTPUT);
  pinMode(MOTOR1_IN2,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);

  Serial.begin(115200);

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

  //config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
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
}
