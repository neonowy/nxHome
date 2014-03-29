/*
  LiquidCrystal Library - Blink
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD and makes the 
 cursor block blink.
 
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
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/LiquidCrystalBlink
 
 */

#include <Wire.h> // necessary, or the application won't build properly
#include <stdio.h>
#include <PCF8583.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int correct_address = 0;
PCF8583 p (0xA0);
int lLvl;
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 4);
  // Print a message to the LCD.
  pinMode(8, INPUT_PULLUP);
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH);
  Serial.begin(9600);
}

void loop() {
  p.get_time();
  lLvl = map(analogRead(A0), 0, 1023, 0, 255);
  if (digitalRead(8) == LOW) {
    digitalWrite(9, HIGH);
    for (int i=0;i<6;i++) {
      char time[50];
      p.get_time();
      sprintf(time, "%02d:%02d:%02d",
	  p.hour, p.minute, p.second);
      lcd.print(time);
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


