/*
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
   * ends to +5V and ground
   * wiper to LCD VO pin (pin 3)
  */

#include <Wire.h>
#include <PCF8583.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
correct_address = 0;
PCF8583 p (0xA0);
int lLvl;
int buttPin = 8;
int backlightPin = 9;
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 4);
  // Print a message to the LCD.
  pinMode(buttPin, INPUT_PULLUP);
  pinMode(backlightPin, OUTPUT);
  digitalWrite(backlightPin, HIGH);
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  p.get_time();
  sensors.requestTemperatures();
  lLvl = map(analogRead(A0), 0, 1023, 0, 255);
  if (digitalRead(buttPin) == LOW && lLvl <= 240) {
    digitalWrite(9, HIGH);
    char time[50];
    for (int i=0;i<6;i++) {
      p.get_time();
      sensors.requestTemperatures();
      sprintf(time, "%02d:%02d:%02d",
      p.hour, p.minute, p.second);
      lcd.setCursor(0,0);
      lcd.print(time);
      lcd.setCursor(-4,2);
      lcd.print(sensors.getTempByIndex(0));
      delay(1000);
      lcd.clear();
    }
    analogWrite(9, 2);
  }
  else {
  if (lLvl > 240) analogWrite(9, 1);
  else if (lLvl > 210) analogWrite(9, 150);
  else analogWrite(9, 255);
  }
  char time[50];
  sprintf(time, "%02d:%02d:%02d",
	  p.hour, p.minute, p.second);
  lcd.print(time);
  // Turn off the blinking cursor:
   // Turn on the blinking cursor:
  delay(1000);
  Serial.println(lLvl);
  lcd.clear();  
}


