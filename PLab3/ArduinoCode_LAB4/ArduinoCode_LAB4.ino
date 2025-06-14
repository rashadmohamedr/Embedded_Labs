//Arduino code:

#include <SoftwareSerial.h>
#include <Wire.h>

// Pin definitions
const int lm35Pin = A0;        // LM35 sensor pin
const int acs712Pin = A1;      // ACS712 sensor pin
const int voltagePin = A2;     // Voltage sensor pin

// Threshold values
const float tempThreshold = 35.0;  // Temperature threshold in Celsius
const float currentThreshold = 0.5; // Current threshold in Amps
const float voltageThreshold = 12.0; // Voltage threshold in Volts

  int temperature ;
  int current ;
  int voltage ;
void setup() {
  Serial.begin(9600);        // Start serial communication with PC 
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.print("Slave Ready");
}

void loop() {
  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
  // Read sensor values
  temperature = readLM35();
  current = readACS712();
  voltage = readVoltage();

  temperature = temperature * 100;
  current = current * 100;
  voltage = voltage * 100;

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Current: ");
  Serial.println(current);
  Serial.print("Voltage: ");
  Serial.println(voltage);


  delay(1000); // Delay for a second
}

// Function to read temperature from LM35
float readLM35() {
  int lm35Value = analogRead(lm35Pin);
  float voltage = lm35Value * (5.0 / 1023.0);
  return voltage * 100.0;  // Convert to Celsius
}

// Function to read current from ACS712
float readACS712() {
  int acs712Value = analogRead(acs712Pin);
  float voltage = (acs712Value / 1023.0) * 5.0;
  float current = (voltage - 2.5) / 0.185;  // ACS712 gives a 2.5V offset, and 185mV/A sensitivity
  return current;
}

// Function to read voltage from voltage sensor
float readVoltage() {
  int voltageValue = analogRead(voltagePin);
  return (voltageValue * (5.0 / 1023.0)) * (5.0 / 0.5);  // Assuming the voltage divider ratio
}
void requestEvent() {
  // Send sensor data to NodeMCU thourgh i2c
  Wire.write(1);
  Wire.write(temperature);
  Wire.write(2);
  Wire.write(current);
  Wire.write(3);
  Wire.write(voltage);
}