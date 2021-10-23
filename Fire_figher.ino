#include <Arduino.h>
int redLight = 13;// 13 no. pin for redlight
int greenLight = 11;// 11 no.pin for greenlight
int smokeA = A0;//A0 analog pin for smoke sensor
int buzzer = 10;// 10 no. pin for buzzer
int thresSensor = 700;// adjust this relatively
//output and input pin setup. usable pins remaining are 2,3,4,5,6,7,8,9,12(9 pins)
void setup() {
pinMode(redLight,OUTPUT);
pinMode(greenLight,OUTPUT);
pinMode(buzzer,OUTPUT);
//input from smoke sensor, output in buzzer, led and bluetooth
pinMode(smokeA,INPUT);
Serial.begin(9600);
 
}
/*This function detects smoke and rings buzzer. greenlight on means no smoke detected. 
if smoke is detected, then buzzer will ring and red light will be on.
*/
 
void smoke_detection_and_buzzer(){
  int analogSensr = analogRead(smokeA);// reading analog reading of smoke sensor
  if(analogSensr>thresSensor){ // if smoke reading is > 700 then buzzer will ring
      digitalWrite(redLight,HIGH);
      digitalWrite(greenLight,LOW);
      tone(buzzer,700,1000);
      Serial.print(analogSensr);
      Serial.println(" ,YES"); //prints reading and "YES" on the phone via bluetooth
  }
  else
  {
    Serial.print("No Smoke"); // if smoke reading is <= 700 then greenlight will be on
    Serial.println(analogSensr); 
     digitalWrite(redLight,LOW);
     digitalWrite(greenLight,HIGH);
    noTone(buzzer);
  }
}
 
void loop() {
  
 
  smoke_detection_and_buzzer();//for smoke detection
  delay(1000);
  
}
