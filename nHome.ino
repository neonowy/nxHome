#include <Wire.h>
#include <PCF8583.h>
#include <studio.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>


#define ONE_WIRE_BUS 7
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t bufLen = VW_MAX_MESSAGE_LEN;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int correct_address = 0;
LiquidCrystal lcd(12, 6, 5, 4, 10, 2);
PCF8583 p (0xA0);
int ButtPin = 8;
int BuzzPin = 13;
int BacklightPin = 3;
int LDRpin = A0;
int letters = 0;
int beepsCounter = 0;
String message;
byte bell[8] = {
        B00100,
        B01110,
        B01110,
        B11111,
        B11111,
        B00100,
        B00000
     };

byte degree[8] = {
        B00111,
        B00101,
        B00111,
        B00000,
        B00000,
        B00000,
        B00000
     };

void greetings() {
    char powitanie[] =  "Witaj Bartek !";
    lcd.setCursor(0,1);
    lcd.print(powitanie);
    delay(1500);
    lcd.clear();
    
    for(int i=0;i<=1;i++) { 
        lcd.setCursor(i,1);
        lcd.print(powitanie);
        delay(250);
        lcd.clear();
    
        if(i == 1) {
            int x = i+1;
            for(int j = 14;j>0;j--) {
                lcd.setCursor(x,1);
                //lcd.print(powitanie.substring(0,j));
                for(int z = 0;z<j;z++) {
                    lcd.print(powitanie[z]);
                }
                delay(250);
                lcd.clear();
                x++;
            }
        }
    }
}

// getDate function
// Returns date in char array.
// Parameters: object of PCF8583 class
// Return: date in char array (DD/MM/YY)
char* getDate(PCF8583 p) {
    p.get_time();
    char date[50];
    sprintf(date, "%02d/%02d/%02d",
    p.day, p.month, p.year);
    return(date);
}

// getDayOfWeek function
// Returns day of week in char array.
// Parameters: object of PCF8583 class
// Return: Day of week in char array
char* getDayOfWeek(PCF8583 p) {
    p.get_time();
    int dayOfWeek = p.get_day_of_week();
    switch(dayOfWeek) {
    case 1:
        return "Poniedzialek";
        break;
    case 2:
        return "Wtorek";
        break;
    case 3:
        return "Sroda";
        break;
    case 4:
        return "Czwartek";
        break;
    case 5:
        return "Piatek";
        break;
    case 6:
        return "Sobota";
        break;
    case 7:
        return "Niedziela";
        break;
    default:
        return "Error";
    }
}
// getTime function
// Returns time in char array.
// Parameters: object of PCF8583 class
// Return: time in char array (HH:MM:SS)
char* getTime(PCF8583 p) {
    p.get_time();
    char time[50];
    sprintf(time, "%02d:%02d:%02d",
    p.hour, p.minute, p.second);
    return(time);
}

// setBacklight function
// Gets light level form LDR (via ADC) and set LCD backligt level (via PWM).
// Parameters: LDR pin
// Return: none
void setBacklight(int LDRpin, int ButtPin, int BacklightPin, int darkLvl, int mediumLvl) {
    int lLvl = map(analogRead(LDRpin), 0, 1023, 0, 255);
    int backlightLvl;
    if (lLvl > darkLvl) backlightLvl = 1;
    else if (lLvl > mediumLvl) backlightLvl = 150;
    else backlightLvl = 255;
    if (digitalRead(ButtPin) == LOW) digitalWrite(BacklightPin, HIGH);
    else analogWrite(BacklightPin, backlightLvl);
}

void printHome() {
    lcd.setCursor(0,0);
    lcd.print(getDate(p));
    lcd.setCursor(0,1);
    lcd.print(getTime(p));
    lcd.setCursor(-4,2);
    lcd.print(getDayOfWeek(p));
    lcd.setCursor(15,0);
    reloadBell();
    lcd.setCursor(-4,3);
    sensors.requestTemperatures(); 
    lcd.print(sensors.getTempCByIndex(0));
    lcd.setCursor(0,3);
    lcd.write(byte(1));
    lcd.print("C");
}

void receive() {
    if (vw_get_message(buf, &bufLen)) {
        message = "";
        for(int i = 0; i < bufLen; i++) {
            message += char(buf[i]);
        }
        
    }
}

void reloadBell() {
    if (message == "lette" && beepsCounter < 3 || message == "letter" && beepsCounter < 3) {
        lcd.write(byte(0));
        Serial.println("foo!");
        digitalWrite(BuzzPin, HIGH);
        delay(500);
        digitalWrite(BuzzPin, LOW);
        beepsCounter++;
        Serial.println(beepsCounter);
    } else if (message == "empty") {
        lcd.print(" ");
        digitalWrite(BuzzPin, LOW);
        beepsCounter = 0;
    } else {
        digitalWrite(BuzzPin, LOW);
    }
}

void setup() {
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);  // Bits per sec
    vw_rx_start();
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 4);
    pinMode(ButtPin, INPUT_PULLUP);
    pinMode(BacklightPin, OUTPUT);
    pinMode(LDRpin, INPUT);
    pinMode(BuzzPin, OUTPUT);
    digitalWrite(BacklightPin, HIGH);
    sensors.begin(); 
    sensors.setResolution(9);  
    lcd.createChar(0, bell);  
    lcd.createChar(1, degree);        
        
    Serial.begin(9600);
    sensors.begin();
    greetings();
    lcd.clear();
}

void loop() {
    setBacklight(LDRpin, ButtPin, BacklightPin, 240, 210);
    printHome();
    receive();
    Serial.print(message);
    delay(500);
}