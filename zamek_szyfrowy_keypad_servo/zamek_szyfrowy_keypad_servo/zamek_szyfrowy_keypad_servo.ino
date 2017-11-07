// Przerobiony projekt "--- zamek szyfrowy ... by wk@warcaby.lh.pl" --------------
//dodano obsluge lcd oraz servo
/* OPIS:
------------------------------------------------------------
- Klawiatura numeryczna 4X4 połączona do pinów 2-9 Arduino
- Pin 10 do połączenia BUZERA
- Pin 11 - wyjście sterujące zamkiem
- szyfr 5-cio znakowy 
- zmiana szyfru po wpisaniu kombinacji: *#0#* ( lub innej dowolnej 
 5-cio znakowej - patrz w programie )
- do konfiguracji i sprawdzenia działania połacz Serial Terminal (kabel USB)
---------------------------------------------------------- */
#include <Keypad.h>
#include<EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
Servo myServo;

// --- zmiennie ------------------------------------------
const byte wiersze= 4; //ilość wierszy
const byte kolumny= 4; //ilość kolumn
//definicja klawiszy (identycznie jak na klawiaturze)
char mapaklawiszy[wiersze][kolumny]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte wiersze_piny[wiersze] = {9,8,7,6}; // wiersze 0-3
byte kolumny_piny[kolumny]= {5,4,3,2}; // kolumny 0-3
int pamiec[5]; // pamięc wciśniętych klawiszy - 5 znaków
volatile int szyfr=0; // szyfr=0 -> brak szyfr=1 jest aktywny
volatile int l=0; // zmienna pomocnicza, licznik znaków
volatile int result=0; // rezultat porównania ciągu 5-ciu znaków
int mykey[5]; // taklica wciśnietych znaków
Keypad myKeypad= Keypad(makeKeymap(mapaklawiszy), wiersze_piny, kolumny_piny,
 wiersze, kolumny); 
// definicja obiektu klasy Keypad
// --- funkcje programu: --------------------------------- 
void warning_0() 
  {
    digitalWrite(10,1);delay(600);digitalWrite(10,0);delay(30);
    warning_1();
  } // bipper dla OK
void warning_1() 
  {
    digitalWrite(10,1);delay(30);digitalWrite(10,0);delay(30);
  } // bipper dla wrong code
void odczyt_EEPROM() // funkcja sprawdza obecnośc szyfru w pamięci EEPROM
  {
   for (int i=0;i<5;i++) {pamiec[i]=EEPROM.read(i);} 
   if(pamiec[0]!=255 && pamiec[1]!=255 && pamiec[2]!=255 && pamiec[3]!=255 
   && pamiec[4]!=255) szyfr=1; else szyfr=0; 
   // jeśli jest KOD to szyfr =1 else szyfr =0
  }
void fnowy_kod() // funkcja ustawia nową kombinację dla szyfru
  {
   for(int i=0;i<5;i++)EEPROM.write(i,255); //kasowanie EEPROM
   lcd.setCursor(0,0);
   lcd.print("Skasowano stary");
   lcd.setCursor(0,1);
   lcd.print("kod dostepu");
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Wprowadz nowy");
   lcd.setCursor(0,1);
   lcd.print("kod dostepu");
   while(l<5)
     {
     char keypressed = myKeypad.getKey();
     if (keypressed != NO_KEY)
       {
       EEPROM.write(l,keypressed);l++; // zapis pojedynczego znaku
       lcd.print(keypressed);warning_1(); // komunikacja po RS
       } 
     }
   for(int k=0;k<3;k++){
   warning_1();warning_1();warning_1();warning_1();warning_1();delay(300);}
   Serial.println(" ---> Kod dostepu zapisany");l=0; 
  }
void dostep() // funkcja sterująca zamkiem
  {
   lcd.setCursor(0,0);
   lcd.print("Wprowadz KOD");
   lcd.setCursor(0,1);
   lcd.print("dostepu:"); 
   while(l<5)
     {
     char keypressed = myKeypad.getKey();
     if (keypressed != NO_KEY)
       {
       mykey[l]=keypressed;l++;lcd.print(keypressed);warning_1();
       } 
     }
   if( mykey[0]==pamiec[0] && mykey[1]==pamiec[1] && mykey[2]==pamiec[2] 
   && mykey[3]==pamiec[3] && mykey[4]==pamiec[4] )
     {
     warning_1();warning_1();
     lcd.setCursor(0,0);
     lcd.print(" OK --> OTWIERAM ZAMEK");
     lcd.setCursor(0,1);
     lcd.print("na 3 sekundy !!");
     myServo.write(170); delay(3000);myServo.write(10); 
     // otwieram zamek na 3 sek 
     lcd.clear();
     } 
   // sprawdzamy czy wybrano specjalny kod do zmiany szyfru 
   else 
     {
       result++;
       if(mykey[0]=='*' && mykey[1]=='#' && mykey[2]=='0' && mykey[3]=='#' 
       && mykey[4]=='*' )
         {
         // *7#AC 
         for(int k=0;k<3;k++){
         warning_1();warning_1();warning_1();warning_1();warning_1();
         delay(300);}
         lcd.setCursor(0,0);
         Serial.println(" ");
         lcd.print("Wykryto polecenie");
         lcd.setCursor(0,1);
         lcd.print("zmiany kodu");
         lcd.setCursor(0,0);
         lcd.print("wykonuje procedure");
         lcd.setCursor(0,1);
         lcd.print("zmiany kodu");
         delay(500);l=0;fnowy_kod();
         } 
       else result++;
     }
   if(result>1){ lcd.setCursor(0,0);lcd.print("Kod");lcd.setCursor(0,1);lcd.print("NIEPRAWIDLOWY");
   warning_0();delay(500);result=0;}
   l=0; 
  lcd.clear();
  }
//-------------------------------------------------------
void setup()
 {
   pinMode(10,OUTPUT);
   pinMode(11,OUTPUT);
   digitalWrite(10,0);
   digitalWrite(11,0);
   Serial.begin(9600);
   Serial.flush();
   lcd.begin(16,2);
   lcd.backlight();
   myServo.attach(11);
   myServo.write(101);
 }
//--------------------------------------------------------
void loop()
 {
   odczyt_EEPROM();delay(30);
   if(szyfr==0)fnowy_kod();else dostep();
 }
// =========== KONIEC ====================================

