#include <math.h>
#include <LiquidCrystal.h>
#include <DigiPotX9Cxxx.h>
// #include <cstdlib>
using namespace std;
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
  lcd.setCursor(0, 0);
  lcd.print("  FORCE SENSOR");
  pot.set(20);

}

void loop()
{
  float analog_value = analogRead(A0);
  inputVoltage = (analog_value * 5.0) / 1024.0; 
  float POT_VALUE=pot.get();
  if(inputVoltage<=4.5 and inputVoltage>=3.5){
    Serial.print(inputVoltage);
    Serial.print(" ");
    float filtered_voltage=Kalman(inputVoltage);
    Serial.print(filtered_voltage);
    float resistance1=force_resistance(filtered_voltage);
    Serial.print(" ");
    Serial.println(resistance1);
    float force=force_cal(resistance1);
    to_LCD(force);
    setresistance();
  }
  else if(inputVoltage>=4.5){
    decrese_resistance();
  }
  else if(inputVoltage<=3.5 and POT_VALUE<80){
    increase_resistance();
  }
  else{
    to_LCD(0.00);
  }
}

//############################################################ End of loop ##################################################


//################################################### FR Resistance ##################################

float force_resistance(float vout)
{
    float pot_res=pot.get();
    return 8*pot_res/vout;
}
//####################################################### END OF FR Resistance ###########################

///######################################### Calculation of Force #############################

float force_cal(float res)
{
  if(res<=330 and res>=107){
    return 3.3848-0.0074*res;
  }
  if(res<107 and res>=73){
    return 12.23-0.0876*res;
  }
  if(res<73){
    return 62.26-0.7845*res;
  }
  if(res>330){
    return 0.00;
  };
}

void increase_resistance()
{
  pot.increase(2.5);
}

void decrese_resistance()
{
  pot.decrease(2.5);
}

void to_LCD(float vout)
{
  lcd.setCursor(0, 1);
  lcd.print("Force = ");
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

void resistance()
{
   if (Serial.available()){
    num=Serial.parseInt();
    if(num==1){
      increase_resistance();
    }
    if(num==0){
      decrese_resistance();
    }
   }
}

void setresistance()
{
  if(Serial.available()){
    num=Serial.parseFloat();
    pot.set(num);
  }
}