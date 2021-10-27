#include <Arduino.h>
#include<Servo.h>
Servo Myservo; // servo motor variable
// pins for bluetooth 0 1
// pins for flame detector : 2 3 4 
// pins for smoke detector : A0 A1
// pins for water pump : A5
// pin for buzzer = 10
// pin for servo motor  = 11
// servo position  : 50 degree  : left , 130degree : mid , 180 degree: right 
int servoLeftPos = 50;  
int servoMidPos = 130;
int servoRightPos = 180;
int servoCurrentPos ;

// flame sensor pins
int leftFlamePin = 4;
int midFlamePin = 3;
int rightFlamePin = 2;

int servoPin = 11; // servo motor pin
int smokeA = A3;		//A0 analog pin for smoke sensor

int buzzer = 10;		// 10 no. pin for buzzer
int thresSensor = 700;		// adjust this relatively
//output and input pin setup. usable pins remaining are 2,3,4,5,6,7,8,9,12(9 pins)

// variables for flame sensor
int flamePin;
int flame_LED;
int flame;

int flameDetected = 0;		// flag if flame is detected;
// time calculation , smokeStartTime : starting time of detecting smoke, flameStartTIme : starting time of detecting flame
unsigned long int smokeStartTime = 0, currentTime, flameStartTime = 0;
int smokeFire = 0;		// check if fire detected through smoke
// zero for no smoke still
// 1 for smoke detected previously , timer counting
// 2 much smoke , make buzzer alarm, send to bluetooth
int minTimeToSetSmokeAlarm = 4000;
int minTimeToSetFlameAlarm = 2000;
unsigned long StartTime = 1002;
unsigned long endTime = 0;
//setup flame sensor
void setup_flame_sensor ()
{
  //flamePin= 2;
  //flame_LED = 12;//
  pinMode (leftFlamePin, INPUT);
  pinMode (midFlamePin, INPUT);
  pinMode (rightFlamePin, INPUT);



  //pinMode(flame_LED,OUTPUT);
  flame = HIGH;
}


// variable for water pump & relay
const int RELAY_PIN = A5;	// the Arduino pin, which connects to the IN pin of relay

// 
void setup_relay_pin ()
{
  // initialize digital pin A5 as an output.
  pinMode (RELAY_PIN, OUTPUT);
}

void setup ()
{


  // pinMode(redLight,OUTPUT);
  //pinMode(greenLight,OUTPUT);
  pinMode (buzzer, OUTPUT);
  //input from smoke sensor, output in buzzer, led and bluetooth
  pinMode (smokeA, INPUT);
  //pinMode (smokeB, INPUT);
  Myservo.attach (servoPin);
  Myservo.write (servoMidPos);
  servoCurrentPos = servoMidPos;
  delay (1000);

  setup_flame_sensor ();
  setup_relay_pin ();
  Serial.begin (9600);


}
// this function rotates the servo motor
void rotateServo(int prevPos, int nextPos)
{
  if(prevPos<nextPos){
  for(int pos=prevPos;pos<=nextPos;pos++){
      Myservo.write(pos);
      delay(15);
  }
  }
  else
  {
    for(int pos=prevPos;pos>=nextPos;pos--){
      Myservo.write(pos);
      delay(15);
  }
  }
}
/*This function detects smoke and rings buzzer. greenlight on means no smoke detected. 
if smoke is detected, then buzzer will ring and red light will be on.
*/

int smoke_detection_and_buzzer ()
{
  int analogSensr = analogRead (smokeA);	// reading analog reading of smoke sensor

  //serial.print ("Smoke sense : Pin A0: ");

  //serial.print (analogSensr);

  if (analogSensr > thresSensor)
    {				// if smoke reading is > 700 then buzzer will ring
      //digitalWrite(redLight,HIGH);
      //digitalWrite(greenLight,LOW);
      //tone (buzzer, 500);
      //Serial.println (" smoke detected YES");	//prints reading and "YES" on the phone via bluetooth
      return 1;
    }
  else
    {
      //Serial.print ("No Smoke");	// if smoke reading is <= 700 then greenlight will be on
      //noTone (buzzer);
      return 0;
    }
}

int detect_flame (int flamePin)
{
  flame = digitalRead (flamePin);
  //serial.println ("flame : ");
  //serial.println (flame);
  if (flame == LOW)
    {
      flameDetected = 1;
      //serial.print (flamePin);
      //Serial.println (" :   *******  fire ******");
      return 1;			// fire detected 
      //digitalWrite(flame_LED,HIGH);
    }

  else
    {
      flameDetected = 0;
      //serial.print (flamePin);
      //Serial.println ("   :  No fire");
      return 0;			// no fire 
      //digitalWrite(flame_LED,LOW);
    }
}


// start pump

void start_water_pump ()
{
  // delay to save pump from sudden on off
  delay (200);

  digitalWrite (RELAY_PIN, HIGH);	// turn on pump 5 seconds
  /*
     delay(1000);
     digitalWrite(RELAY_PIN, LOW);  // turn off pump 5 seconds
     delay(1000);
   */
}

void shut_water_pump ()
{
  delay (2000);			// run water pump for 2 more seconds 
  digitalWrite (RELAY_PIN, LOW);	// turn off pump
}

void detect_fire ()
{
  int flameDetect1, flameDetect2, flameDetect3, smokeDetect1;
  smokeDetect1 = smoke_detection_and_buzzer ();	// check smoke
  // if smoke detected
  if (smokeDetect1)
    {
      // if smoke is not detected before 
      if (smokeStartTime == 0)
	{
	  smokeStartTime = millis ();
	  smokeFire = 1;
	  //Serial.println ("smoke detected for first time ");
	}
      // if smoke is detected before check if smoke was for 4 seconds  
      else if (smokeFire == 1)
	{
	  currentTime = millis ();
	  if ((currentTime - smokeStartTime) >= minTimeToSetSmokeAlarm)
	    {
	      tone (buzzer, 500);
	      Serial.println ("buzzer started : ******** ALARM , SMOKE DETECTED ******** ");
	      smokeFire = 2;
	      Serial.println ("smoke for long time, buzzer & pump started ");
	      start_water_pump ();
	    }
	}


    }
  else
    {
      smokeStartTime = 0;
      smokeFire = 0;
      noTone (buzzer);
      if (flameDetected == 0)
	{
	  shut_water_pump ();
	}
    }
  flameDetect1 = detect_flame (leftFlamePin);
  if (flameDetect1)
    {
      // make servo to rotate to flame 1
      rotateServo(servoCurrentPos,servoLeftPos);
      servoCurrentPos = servoLeftPos;
      //Myservo.write (servoLeftPos);
      //Serial.println (" servo in left position ");
      flameStartTime = millis ();
      start_water_pump ();
      //Serial.println (" Water pump started for flame 1 left ");
      while (flameDetect1)
	{
	  flameDetect1 = detect_flame (leftFlamePin);
	  delay (200);
	  currentTime = millis ();
	  if ((flameStartTime - currentTime) > minTimeToSetFlameAlarm)
	    {
	      Serial.println ("left flame  detected for long time, buzzer started\n\n********** ALARM , FIRE FLAME DETECTED ******* ");

	      tone (buzzer, 500);
	      //Serial.println ("buzzer started 1 ");
	    }
	}
      noTone (buzzer);
      //Serial.println ("buzzer off ");
      flameStartTime = 0;
      shut_water_pump ();
      //Serial.println (" Water pump off ");
    }

  //delay(500);  // delay so that next flame detection function is not called within previous & 
  // the values won;t overlap
  flameDetect2 = detect_flame (midFlamePin);
  if (flameDetect2)
    {
      // make servo to rotate to flame 2
      rotateServo(servoCurrentPos,servoMidPos);
      servoCurrentPos = servoMidPos;
      //Myservo.write (servoMidPos);
      //Serial.println (" servo in mid position ");
      flameStartTime = millis ();
      start_water_pump ();
     // Serial.println (" Water pump started for flame 2 mid ");
      while (flameDetect2)
	{
	  flameDetect2 = detect_flame (midFlamePin);
	  delay (200);
	  currentTime = millis ();
	  if ((flameStartTime - currentTime) > minTimeToSetFlameAlarm)
	    {
	      Serial.println ("mid flame  detected for long time, buzzer started\n\n********** ALARM , FIRE FLAME DETECTED ******* ");
	      tone (buzzer, 500);
	      //Serial.println ("buzzer started 2 ");
	    }
	}
      noTone (buzzer);
      //Serial.println ("buzzer stopped ");
      flameStartTime = 0;
      shut_water_pump ();
     // Serial.println (" Water pump off ");
    }
  //delay(500);
  flameDetect3 = detect_flame (rightFlamePin);

  if (flameDetect3)
    {
      // make servo to rotate to flame 3
      rotateServo(servoCurrentPos,servoRightPos);
      servoCurrentPos = servoRightPos;
      //Myservo.write (servoRightPos);
      //Serial.println (" servo in right position ");
      flameStartTime = millis ();
      start_water_pump ();
      //Serial.println (" Water pump started for flame 3 right ");
      while (flameDetect3)
	{
	  flameDetect3 = detect_flame (rightFlamePin);
	  delay (200);
	  currentTime = millis ();
	  if ((flameStartTime - currentTime) > minTimeToSetFlameAlarm)
	    {
	      Serial.println ("right flame  detected for long time, buzzer started\n\n********** ALARM , FIRE FLAME DETECTED ******* ");
	      tone (buzzer, 500);
	      //Serial.println ("buzzer started 3 ");
	    }
	}
      noTone (buzzer);
      //Serial.println ("buzzer stopped ");
      flameStartTime = 0;
      shut_water_pump ();
    //  Serial.println (" Water pump off ");
    }
  //delay(500);
  /*
     if (flameDetect1 || flameDetect2 || flameDetect3 || smokeDetect1)
     {

     //find which flameDetected, then move the pipe using the servo motor to that direction
     //start_water_pump();


     start_water_pump ();

     Serial.println (" Water pump started ");


     }
     else
     {
     //delay(500);
     shut_water_pump ();
     Serial.println (" Water pump off ");

     }
   */
}

void loop ()
{


  //smoke_detection_and_buzzer();//for smoke detection
  //start_water_pump();
  detect_fire ();


}
