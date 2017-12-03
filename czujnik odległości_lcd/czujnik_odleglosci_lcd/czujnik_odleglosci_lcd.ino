/* Ultrasonic HC-SR04 and lcd with I2C */

#include "Ultrasonic.h"
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Ultrasonic ultrasonic(12,13); //Ultrasonic ultrasonic(Trig,Echo);

void setup() {
Serial.begin(9600);
  lcd.begin(16,2);   
  lcd.backlight();  
  lcd.setCursor(0,0); 
  lcd.print("cm");
  
}

void loop()
{
  lcd.backlight();
  lcd.setCursor(8,0);
  lcd.print(ultrasonic.Ranging(CM)); 
  lcd.print("cm");
  lcd.print("    ");
   
    
  delay(100);
}




