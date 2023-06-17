// C++ code
#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);

float inputVoltage=0.0;

float temp=0.0;

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
  Serial.println(inputVoltage);
  lcd.setCursor(0, 1);
  lcd.print("Voltage= ");
  lcd.print(inputVoltage);
}
