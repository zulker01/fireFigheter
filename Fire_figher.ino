#include <Arduino.h>

// pins for bluetooth 0 1
// pins for flame detector : 2 3 4 
// pins for smoke detector : A0
// pins for water pump : A5

int leftFlamePin = 2;
int midFlamePin = 3;
int rightFlamePin = 4;
int highPin = 13;

int redLight = 13;// 13 no. pin for redlight
int greenLight = 11;// 11 no.pin for greenlight
int smokeA = A0;//A0 analog pin for smoke sensor
int buzzer = 10;// 10 no. pin for buzzer
int thresSensor = 700;// adjust this relatively
//output and input pin setup. usable pins remaining are 2,3,4,5,6,7,8,9,12(9 pins)

// variables for flame sensor
int flamePin;
int flame_LED;
int flame;


 unsigned long StartTime = 1002;
  unsigned long endTime = 0;
//setup flame sensor
void setup_flame_sensor()
{
   //flamePin= 2;
   //flame_LED = 12;//
   pinMode(leftFlamePin,INPUT);
   pinMode(midFlamePin,INPUT);
   pinMode(rightFlamePin,INPUT);
   pinMode(highPin, OUTPUT);
   
   //pinMode(flame_LED,OUTPUT);
   flame = HIGH;
}


// variable for water pump & relay
const int RELAY_PIN = A5;  // the Arduino pin, which connects to the IN pin of relay

// 
void setup_relay_pin()
{
  // initialize digital pin A5 as an output.
  pinMode(RELAY_PIN, OUTPUT);
}

void setup() {

  
 // pinMode(redLight,OUTPUT);
  //pinMode(greenLight,OUTPUT);
  pinMode(buzzer,OUTPUT);
  //input from smoke sensor, output in buzzer, led and bluetooth
   pinMode(smokeA,INPUT);

  
  setup_flame_sensor();
  setup_relay_pin();
  Serial.begin(9600);

 
}
/*This function detects smoke and rings buzzer. greenlight on means no smoke detected. 
if smoke is detected, then buzzer will ring and red light will be on.
*/
 
void smoke_detection_and_buzzer(){
  int analogSensr = analogRead(smokeA);// reading analog reading of smoke sensor
  
  Serial.print("Smoke sense : Pin A0: ");
  
  Serial.print(analogSensr);
      
  if(analogSensr>thresSensor){ // if smoke reading is > 700 then buzzer will ring
      //digitalWrite(redLight,HIGH);
      //digitalWrite(greenLight,LOW);
      tone(buzzer,700,1000);
      Serial.println(" ,YES"); //prints reading and "YES" on the phone via bluetooth
  }
  else
  {
    Serial.print("No Smoke"); // if smoke reading is <= 700 then greenlight will be on
     digitalWrite(redLight,LOW);
     digitalWrite(greenLight,HIGH);
    noTone(buzzer);
  }
}

int detect_flame(int flamePin)
{
  flame = digitalRead(flamePin);
  Serial.println("flame : ");
  Serial.println(flame);
  if (flame== LOW)
  {
    Serial.print(flamePin);
    Serial.println(" :   *******  fire ******");
    return 1 ; // fire detected 
    //digitalWrite(flame_LED,HIGH);
  }
  
  else{
    Serial.print(flamePin);
    Serial.println("   :  No fire");
    return 0 ; // no fire 
    //digitalWrite(flame_LED,LOW);
  }
}


// start pump

void start_water_pump()
{
  digitalWrite(highPin,HIGH);
  digitalWrite(RELAY_PIN, HIGH); // turn on pump 5 seconds
  /*
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);  // turn off pump 5 seconds
  delay(1000);
  */
}

void shut_water_pump()
{
  digitalWrite(RELAY_PIN, LOW);  // turn off pump
}
void detect_fire()
{
  int flameDetect1 ,flameDetect2,flameDetect3;
  flameDetect1 = detect_flame(leftFlamePin);
  //delay(500);  // delay so that next flame detection function is not called within previous & 
             // the values won;t overlap
  flameDetect2 = detect_flame(midFlamePin);
  //delay(500);
  flameDetect3 = detect_flame(rightFlamePin);
  //delay(500);
  if(flameDetect1||flameDetect2||flameDetect3)
  {
    //start_water_pump();
   
     start_water_pump();
    Serial.println(" Water pump started ");
    
    
  }
  else
  {
    //delay(500);
    shut_water_pump();
    Serial.println(" Water pump off ");
    
  }
}
void loop() {
  
 
  //smoke_detection_and_buzzer();//for smoke detection
  //start_water_pump();
  detect_fire();
  
  
}
