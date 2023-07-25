#include <math.h>
#include <LiquidCrystal.h>
#include <DigiPotX9Cxxx.h>

//############################################## Components ###########################################

LiquidCrystal lcd(8,9,4,5,6,7);
DigiPot pot(10,11,12);

//############################################## Initial Variable Values ##############################

float inputVoltage=0.0;
float temp=0.0;
int num;
float value;
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
  to_LCD(inputVoltage);
  rand_signal();
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

    if(POT_VALUE<num){
      while(num>POT_VALUE){
        increase_resistance();
      }
    }

    else{
      while(num<POT_VALUE){
        decrese_resistance();
      }
    }

    }  
}

//#################################################### KALMAN FILTER ##################################################

int ESTIMATION=0;
float MEASUREMENT_ERROR=0.1;
float VARIANCE=4;

//Prediction
float X0=0;
float P0=VARIANCE;

float X1=0;
float P1=VARIANCE;

float Kalman(float measurement)
{
    //Iteration
    float R1=pow(MEASUREMENT_ERROR,2);
    float K=P1/(P1+R1);

    X1=X1+K*(measurement-X1);

    P1=(1-K)*P1;

    return X1;
}

void rand_signal(){
    if (Serial.available()){
      value=Serial.parseFloat();
      float predicted=Kalman(value);
      Serial.print(value);
      Serial.print("  ");
      Serial.println(predicted);
    }
}

//#############################################################################################################################


