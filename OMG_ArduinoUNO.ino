#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial ArduinoUno(0, 1); // RX, TX

int n;
int red=9; //green
int blue=5;
int green=6; //red

volatile unsigned long cnt = 0;
unsigned long oldcnt = 0;
unsigned long t = 0;
unsigned long last;

void irq1()
{
  cnt++;
}

void setup() {
// put your setup code here, to run once:

  ArduinoUno.begin(115200);
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  digitalWrite(red,LOW);
  digitalWrite(blue,HIGH);
  digitalWrite(green,LOW);
  lcd.begin();  // initialize the LCD
  lcd.backlight();   // Turn on the blacklight and print a message.
  Serial.println("START");

  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  attachInterrupt(0, irq1, RISING);

}

void loop() {
  if (millis() - last >= 1000)
  {
    last = millis();
    t = cnt;
    unsigned long hz = t - oldcnt;

    n= (hz+50)/100;
    ArduinoUno.print(n);  // +50 == rounding last digit
    ArduinoUno.println("\n");
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FREQ: "); 
    lcd.print(hz);
    lcd.setCursor(0,1);
    lcd.print("    = "); 
    lcd.print((hz+50)/100);  // +50 == rounding last digit
    lcd.print(" mW/m2");
    
    delay(1000);
    oldcnt = t;
  }
}
