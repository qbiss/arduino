#include <DHT.h>
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  
float wilgotnosc; 
float temperatura; 
#define DHTPIN 8 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE); 

void setup()  
{
  Serial.begin(9600);
  lcd.begin(16,2);   
  lcd.backlight();  
  dht.begin ();
  lcd.setCursor(0,0); 
  lcd.print("TEMP");
  lcd.setCursor(0,1); 
  lcd.print("WILG");

}

void loop() 
{
   temperatura = dht.readTemperature();
    wilgotnosc = dht.readHumidity();
   
   lcd.backlight(); 
   lcd.setCursor(8,0);
   lcd.print(temperatura);
   lcd.print("*C");
   lcd.setCursor(8,1);
   lcd.print(wilgotnosc);
   lcd.print("%");
}
