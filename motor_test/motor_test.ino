int motorpin = A5;


// variable for water pump & relay
const int RELAY_PIN = A5;  // the Arduino pin, which connects to the IN pin of relay

// 
void setup_relay_pin()
{
  // initialize digital pin A5 as an output.
  pinMode(RELAY_PIN, OUTPUT);
}


// start pump

void start_water_pump()
{
  digitalWrite(RELAY_PIN, HIGH); // turn on pump 5 seconds
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);  // turn off pump 5 seconds
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
 // pinMode(motorpin, OUTPUT);
 setup_relay_pin();
}

void loop() {

  start_water_pump();
/*
  digitalWrite(motorpin, HIGH);
  delay(1000);
  // put your main code here, to run repeatedly:

  digitalWrite(motorpin, LOW);
  delay(1000);
  */
}
