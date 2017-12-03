#define przycisk1 9
#define przycisk2 10
#define d 3000
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Wire.h>
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DS3231 rtc(SDA, SCL);
float wilgotnosc;
float temperatura;
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  dht.begin ();
  rtc.begin ();
  lcd.setCursor(0,0);
  lcd.print("Godz:   ");
  lcd.setCursor(0,1);
  lcd.print("Data:   ");
  lcd.noBacklight ();
  pinMode (przycisk1, INPUT_PULLUP);
  pinMode (przycisk2, INPUT_PULLUP);
  
  
}

void loop() {
  switch ((digitalRead(przycisk1) == LOW)) 
   {
    case 1:
      //instrukcje dla var=1
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print("TEMP");
      lcd.setCursor(0,1);
      lcd.print("WILG");
      lcd.setCursor(8,0);
      lcd.print(temperatura);
      lcd.print("*C");
      lcd.setCursor(8,1);
      lcd.print(wilgotnosc);
      lcd.print("%");
      delay(d);
      lcd.clear();
      break;
        default: 
      // instrukcje domyślne (jeżeli var różne od 1 i 2)
             lcd.setCursor(6,0);
             lcd.print(rtc.getTimeStr());
             lcd.setCursor(6,1);
             lcd.print(rtc.getDateStr());
             
   }
      switch ((digitalRead(przycisk2) == LOW)) 
   {
    case 1:
      //instrukcje dla var=1
      lcd.backlight();
      delay(d);
      
              break;
        default: 
      // instrukcje domyślne (jeżeli var różne od 1 i 2)
      lcd.noBacklight();
  }

temperatura = dht.readTemperature();
wilgotnosc = dht.readHumidity();
}
