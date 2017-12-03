// example IC 2262/2272 4 Channel 315Mhz Key Wireless Remote Control Kits 

void setup() {
  // put your setup code here, to run once:
pinMode(9, INPUT_PULLUP);
pinMode(10,OUTPUT);
pinMode (11, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

    if (digitalRead(9) == HIGH)
{
    digitalWrite(10, HIGH);
}

else if (digitalRead(11) == HIGH)
     
{
  
    digitalWrite(10, LOW);
 }
}
