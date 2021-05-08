#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
int VoltageDiv = A0;    // select the input pin for the voltmeter
int VDiv_Value = 0;  // variable to store the value coming from the sensor
float PV_Volt = 0; // variable to store wattage of PV panel
float PV_miliWatt = 0; // variable to store wattage of PV panel

float F_off[30];    //TODO: REMOVE THESE OR MAKE USE OF THEM
float F_on[30];
File off_file;
File on_file;

const int ledPin = 8;    
int ledState = LOW;

const int buttonPin = 2;
int buttonState = 0;

const int CS_pin = 10;    // This is the CS pin on microsd card module

int set=0;

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.init();
  lcd.backlight();
  
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Push to start");
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT); 
  buttonState = digitalRead(buttonPin);
  
  while (buttonState == LOW)
  {
      delay(1);
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH)
      {
          continue;
      }
  }
  
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("PV Watt Produced;");
  lcd.setCursor(0, 1);
  lcd.print("="); 
  
  SD.begin(CS_pin);  
  if (SD.exists("off_file.txt") || SD.exists("on_file.txt"))
  {
    SD.remove("*.txt");        //TODO: CHECK IF THIS IS WORKING
  };
  
}

void loop()
{
  set = set + 1;
  
  if (set = 1)
  {
      off_file = SD.open("off_file.txt", FILE_WRITE);
  }
  else if (set = 2)
  {
      on_file = SD.open("on_file.txt", FILE_WRITE);
  };
  
  switch (set)
  {
      case 1:
        for (int i = 0; i <= 30; i++)
          {
            delay(1000);
            VDiv_Value = (analogRead(VoltageDiv)); // Since PV panel's max output is 6V this is expected to measure 3V@max
            PV_Volt = 2*((VDiv_Value*5)/1023.0);
            PV_miliWatt = 1000*(pow(PV_Volt, 2)/2000.0);
            // analogRead func. maps measured voltage between 0 to 1023 thus this conversion 
            //P=V^2/R we know the voltage and resistor values (2x 1kohms)
            lcd.setCursor(2, 1);
            lcd.print(PV_miliWatt);
            off_file.println(PV_miliWatt);
            F_off[i] = PV_miliWatt;
          }
      break;
      
      case 2:
        for (byte i = 0; i <= 30; i++)
          {
            delay(1000);
            VDiv_Value = (analogRead(VoltageDiv)); // Since PV panel's max output is 6V this is expected to measure 3V@max
            PV_Volt = 2*((VDiv_Value*5)/1023.0);
            PV_miliWatt = 1000*(pow(PV_Volt, 2)/2000.0);
            // analogRead func. maps measured voltage between 0 to 1023 thus this conversion 
            //P=V^2/R we know the voltage and resistor values (2x 1kohms)
            lcd.setCursor(2, 1);
            lcd.print(PV_miliWatt);
            on_file.println(PV_miliWatt);
            F_on[i] = PV_miliWatt;
          }
      break;
      
      default:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Restart Arduino");
          delay(100000000);
  }
  
    if (set = 1)
  {
      off_file.close();
  }
  else if (set = 2)
  {
      on_file.close();
  };
  
  if (set = 1)
  {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Start the fan");
      digitalWrite(ledPin, HIGH);
      delay(10000);
      digitalWrite(ledPin, LOW);
  };
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PV Watt Produced;");
  lcd.setCursor(0, 1);
  lcd.print("="); 
  lcd.setCursor(2, 1);
  
}
