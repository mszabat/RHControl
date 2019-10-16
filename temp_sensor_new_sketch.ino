#include "DHT_U.h"//DHT sensor Library
#include <LiquidCrystal_I2C.h>//LCD Library 
#define DHTPIN 6 //Define sensor pin(pin 6)
#define DHTTYPE DHT11 //Type of sensor used declared.

DHT dht(DHTPIN, DHTTYPE);//Create sensor object
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);//Create lcd object using these pins 

boolean a=LOW,b=HIGH;
boolean on=HIGH, off=LOW;
float t=0;
byte h = 0;
byte pwm = 0;
volatile byte set_point;//RH set_point
const byte interruptPin = 2;
const byte interruptPin2 = 3;
long previousMillis = 0;
long interval = 6000; //Read sensor each 6 seconds
long backlighttime = 20000; //lcd backlight on timer in mSeconds
long preMillis = 0;

void setup(){
  
lcd.begin(16, 2);              //Start lcd 
lcd.setCursor(0,0);            //Set in position 0,0
pinMode(interruptPin2, INPUT_PULLUP);
pinMode(interruptPin, INPUT_PULLUP);
pinMode(4, OUTPUT);//Red LED indicating HVAC ON
pinMode(5, OUTPUT);//Blue LED indicating HVAC OFF
attachInterrupt(digitalPinToInterrupt(interruptPin), change_set_point_up, FALLING);
attachInterrupt(digitalPinToInterrupt(interruptPin2), change_set_point_down, FALLING); 
digitalWrite(5,HIGH);//Turn on LED
digitalWrite(4,LOW);//Turn off LED
analogWrite(10, pwm); //~~2% PWM fan drive to detect RH hikes - pin test, thats all for now.
dht.begin();//Start DHT11 sensor
lcd.begin(16, 2);              //Start lcd 
lcd.setCursor(0,0);            //Set in position 0,0
lcd.print("TEMP:");
lcd.setCursor(0,1);//Change cursor position
lcd.print(" RH :");
set_point = 67;
}

void loop(){
 
  int main;
  main = main_loop();

  unsigned long currentbcklight = millis();//time elapsed
  if(currentbcklight - preMillis > backlighttime) //Comparison between the elapsed time and the time in which the action is to be executed
  {
    preMillis = currentbcklight; //"Last time 
    lcd.setBacklight(off);
  }
    else if (digitalRead(2) == LOW) {
      preMillis = millis(); //Keeps the interval at the same length 
      lcd.setBacklight(on);
    }

      else if (digitalRead(3) == LOW) {
        preMillis = millis();
        lcd.setBacklight(on);
      }
    }













//Functions listed below:
  
 
//interrupt set_point_up for setting trigger RH point
int change_set_point_up() {
  set_point ++;
  //lcd.setBacklight(HIGH);
  
}
//interrupt set_point_down for setting triger or lower RH point
int change_set_point_down() {
  set_point --;
  //lcd.setBacklight(HIGH);
}
//Main loop function (so it looks tidy upstairs :) )
int main_loop(){
  unsigned long currentMillis = millis();//time elapsed
  if(currentMillis - previousMillis > interval) //Comparison between the elapsed time and the time in which the action is to be executed
  {
    previousMillis = currentMillis; //"Last time is now"
   // Serial.println(currentMillis);

     h = dht.readHumidity();//humidity value
     t = dht.readTemperature();//temperature value in degrees celcius
      
     lcd.setCursor(5,0);
     lcd.print(t);
     lcd.setCursor(9, 0);//writes over second decimal point to get rid of it (is there a different way for getting only one decimal point??)
     lcd.print("C");
     lcd.setCursor(5,1);
     lcd.print(h);
     lcd.print("%");
     lcd.setCursor(9, 1);
     lcd.print("Trig ");
     lcd.print(set_point);
    
    if(h>set_point && a==LOW)//if humidity is above the set_point and pin a is LOW
    {
      digitalWrite(4,HIGH);//Active air ventilation
      digitalWrite(5,LOW);
      analogWrite(10, 250);
      a=HIGH;
      b=LOW;
     
    }
    else if(h<set_point && b==LOW)//if humidity is below the set_point and Pin b is LOW
    {
      digitalWrite(5,HIGH);
      digitalWrite(4,LOW);//Turn off LED indicator
      analogWrite(10, 5);
      a=LOW;
      b=HIGH;
    }
   

}
}
