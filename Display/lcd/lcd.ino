#include <LiquidCrystal.h>
#include <DigiPotX9Cxxx.h>

LiquidCrystal lcd(8,9,4,5,6,7);

DigiPot pot(10,11,12);

float inputVoltage=0.0;

float temp=0.0;

int num;

int POT_VALUE=0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Voltage");
}

void loop()
{
  float analog_value = analogRead(A0);
  inputVoltage = (analog_value * 5.0) / 1024.0; 
  // Serial.println(inputVoltage);
  to_LCD(inputVoltage);
  manual_resistance();    
}

void increase_resistance()
{
  pot.increase(5);
  POT_VALUE+=5;
}

void decrese_resistance()
{
  pot.decrease(5);
  POT_VALUE-=5;
}

void to_LCD(float vout)
{
  lcd.setCursor(0, 1);
  lcd.print("Voltage= ");
  lcd.print(vout);
}

void manual_resistance()
{
  if(Serial.available())
  {
    num=Serial.parseInt();
    Serial.println(num);
    if(POT_VALUE>num){
      num=2;
    }            
    }  
}




