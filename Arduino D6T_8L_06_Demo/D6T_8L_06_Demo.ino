// I2C D6T-8L-06 Temperature Sensor Demo
// Author: Alexander Göbel
// For more information visit https://www.tindie.com/stores/alegro/ 
// For latest Software visit https://github.com/Voltage-Ranger-Electronics
// Created 09 Mai 2015
// For Arduino Nano w/ ATmega328
// Function     PIN
// SDA          A4
// SCL          A5
// LED          A13

#include <Wire.h>

#define ADRESS  0x0A 
//(Excludes R/W bit)

#define COMAND 0x4C
#define COUNT 19
#define DATA 8
#define WAIT 250
// Minimum wait 125 for 4sps with 2 delays
#define BAUD_RATE 9600
#define LED 13

void setup()
{
  Wire.begin(); 
  Serial.begin(BAUD_RATE);
  pinMode(LED,OUTPUT);
}

byte adress = ADRESS;// Write 0x14, Read 0x15 (Includes R/W bit)
byte comand = COMAND;// Standard command
byte count = COUNT; // Data length read from sensor

byte i=0;// counter
byte z=0;// counter
byte in[COUNT];// Data Input

unsigned int ptat=0;// reference temperature 10 times value of degC
unsigned int p[DATA];// Measure Value
byte pec = 0;//packet error check code based on the "sm bus" specification

void loop()
{
  // get sensor data
  Wire.beginTransmission(adress); // start send write (14h)
  Wire.write(byte(comand));            // send command byte
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(adress, count);
  for(i=0;i<count;i++){
    in[i] = Wire.read();           // read char 0-18 (2*(8+1)+1=19)
  }
  Wire.endTransmission();     // stop
  
  // Get values
  ptat = 0xFF*in[1] +in[0];
  z=2;
  for (i=0;i<(byte)DATA;i++){
    p[i] = 0xFF*(unsigned int)in[z+1] + (unsigned int)in[z];
    z+=2;
  }
  pec = in[count-1];
  
  // print sensor data
  Serial.print("Temp(1=0.1grad): ");
  Serial.print(ptat,DEC);
  Serial.print(", ");
  for (i=0;i<(byte)DATA;i++){
    Serial.print("P");
    Serial.print(i,DEC);
    Serial.print(": ");
    Serial.print(p[i],DEC);
    Serial.print(", ");
  }
  Serial.print("Check: ");
  Serial.println(pec,HEX);
  
  // wait
  delay(WAIT); // Maximum conversion speed 250ms = 4 times per second
  digitalWrite(LED,LOW);
  delay(WAIT);
  digitalWrite(LED,HIGH);
}

