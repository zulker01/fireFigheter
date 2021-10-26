#include<Servo.h>
Servo Myservo;
int pos;
void setup()
{
Myservo.attach(3);
Myservo.write(0);
Serial.begin (9600);
  delay(1000);
}

void loop()
{
  
 
for(pos=0;pos<=180;pos+=10){
Myservo.write(pos);
Serial.println(pos);
delay(1000);
}
  delay(1000);
  
  for(pos=180;pos>=0;pos-=10){
Myservo.write(pos);
Serial.println(pos);
delay(1000);
}
  delay(1000);
  /*
  Myservo.write(90);
  delay(1000);
  Myservo.write(0);
  delay(1000);
  */
}
