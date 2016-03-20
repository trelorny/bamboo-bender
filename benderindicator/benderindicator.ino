/**
 * CI-Monitoring (with bamboo) using Arduino, some LEDs, a rotating light, a little Java Backend and a Futurama Bender mask to give the project a face. Check out the video to get an idea: https://www.youtube.com/watch?v=t6Ux2xYcfyk
 */

#include <Servo.h>
#define rightEyePin 2
#define servoPin 3 //PWM
#define leftEyePin 4
#define blueLedPin 5 //PWM
#define greenLedPin 6 //PWM
#define burningCigarPin 7
//PIN 8
//PWM on PIN 9 disabled due to Servo
//PWM on PIN 10 disabled due to Servo 
#define redLedPin 11 //PWM   
#define relayPin 12
//PIN 13

Servo myservo;

/**
 * The following bool values are keeping the state over an extended timeperiod
 */
bool error = false;
bool lights = false;
bool success = false;
bool building = false;
bool systemenabled = true;
bool masterRed = false;
boolean cigarup = true;

/**
 * Used for parallel port communication
 */
String comdata = "";

/**
 * Servo-Values to indicate a lowered and a raised cigar - adjust if needed!
 */
static int cigarLoweredAngle = 115;
static int cigarRaisedAngle = 35;

/**
 * Enable & Disable demo mode on startup
 */
static boolean demoModeEnabled = false;

void setup (){
  /**
   * Declare the LED Pins - all of them are of type "OUTPUT"
   */
  pinMode(blueLedPin, OUTPUT);  
  pinMode(redLedPin, OUTPUT);  
  pinMode(greenLedPin, OUTPUT);
  pinMode(relayPin,OUTPUT);
  pinMode(leftEyePin,OUTPUT);
  pinMode(rightEyePin,OUTPUT);
  pinMode(burningCigarPin,OUTPUT);

  
    
  //Demo at startup
  if(demoModeEnabled == true){
    demo();
  }
  
  /**
   * Initialize parallel port with default values
   */
  Serial.begin(9600);  
  Serial.print("Your command please:");  
  Serial.println();
  systemenabled = true;
}

void lowerCigar(){
  //Find out wether cigar was lowered already
  int currentServoAngle = myservo.read();
  if(currentServoAngle != cigarLoweredAngle){
    if(myservo.attached() == false){
      myservo.attach(servoPin);
      delay(1000);//wait for a second
    }
    myservo.write(cigarLoweredAngle);
    delay(1000);//wait for a second
    myservo.detach();
    delay(1000);//wait for a second
  }
  //Lighting the cigar
  for(int i = 250;i > 0;i=i-50){
    digitalWrite(burningCigarPin,LOW);
    delay(i);
    digitalWrite(burningCigarPin,HIGH);
    delay(i-25);
  }
  
  digitalWrite(burningCigarPin,HIGH);
}

void raiseCigar(){
  digitalWrite(burningCigarPin,LOW);
  //Find out wether cigar was lowered already
  int currentServoAngle = myservo.read();
  if(currentServoAngle != cigarRaisedAngle){
    if(myservo.attached() == false){
      myservo.attach(servoPin);
      delay(1000);//wait for a second
    }
    myservo.write(cigarRaisedAngle);
    delay(1000);//wait for a second
    myservo.detach();
    delay(1000);//wait for a second
  }
}

void loop(){
  //read string from serial monitor
  if(Serial.available()>0)  // if we get a valid byte, read analog ins:
  {  
    comdata = "";
    while (Serial.available() > 0)  
    {        
      comdata += char(Serial.read());
      delay(2);
    }
    Serial.println(comdata);
  }

  //Determine submitted commands
  if(comdata.indexOf("BUILDING_ON") > -1){
    building = true;
  } 
  if(comdata.indexOf("BUILDING_OFF") > -1) {
    building = false;
  } 
  if (comdata.indexOf("ERROR_ON") > -1){
    error = true;
    success = false;
  } 
  if (comdata.indexOf("SUCCESS_ON") > -1) {
    success = true;
    error = false;
  }
  if(comdata.indexOf("LIGHTS_ON") > -1) {
    lights = true;
  } 
  if(comdata.indexOf("LIGHTS_OFF") > -1) {
    lights = false;
  }
  if(comdata.indexOf("MASTER_RED_ON") > -1){
    masterRed = true;
  }
  if(comdata.indexOf("MASTER_RED_OFF") > -1){
    masterRed = false;
  }
  if(comdata.indexOf("LOWER_CIGAR") > -1){
    cigarup = false;
  }
  if(comdata.indexOf("RAISE_CIGAR") > -1){
    cigarup = true;
  }
  if(comdata.indexOf("INDICATORS_OFF") > -1){
    systemenabled = false;
  } 
  if(comdata.indexOf("INDICATORS_ON") > -1){
    systemenabled = true;
  }

  if (systemenabled == true){    
    if(lights == true){
      digitalWrite(relayPin, HIGH); //Close the relay
    } else {
      digitalWrite(relayPin, LOW);//Open the relay
    }
    if(masterRed == true){
      eyesRed();
    } else {
      analogWrite(leftEyePin,0);
      analogWrite(rightEyePin, 0);
    }
    if(cigarup == true){
      raiseCigar();
    } else {
      lowerCigar();
    }
    if(building == true){
      blinkBuildingLED();  
    } else {
      shineLed(); 
    }
  } else {
    analogWrite(redLedPin, 0); 
    analogWrite(greenLedPin, 0); 
    analogWrite(blueLedPin, 0); 
    analogWrite(leftEyePin,0);
    analogWrite(rightEyePin, 0);
    digitalWrite(relayPin, LOW);
    raiseCigar();
    blinkBlueLED();
  }
}


void eyesRed(){
    analogWrite(leftEyePin, 255);
    analogWrite(rightEyePin, 255);
}

void shineLed(){
  int activeLed = findActiveLed();
  
  //Switch all LEDs off
  if(activeLed != greenLedPin){
    analogWrite(greenLedPin, 0); 
  }
  if(activeLed != redLedPin){
    analogWrite(redLedPin, 0); 
  }
  if(activeLed != blueLedPin){
    analogWrite(blueLedPin, 0);
  }
  //Switch on active State-Led
  analogWrite(activeLed, 128);
}

int findActiveLed(){
  int activeLed = 0;  
  if(success == true){
    activeLed = greenLedPin;
  } else if(error == true){
    activeLed = redLedPin;
  } 
  
  return activeLed;
}

void blinkBuildingLED (){    
  int delaytime = 4;
  analogWrite(greenLedPin, 0);
  analogWrite(redLedPin, 0);
  analogWrite(blueLedPin, 0);
  int activeLed = findActiveLed();  
  
  for (int a=16; a<=255;a++){
    analogWrite(activeLed, a);
    delay(delaytime);
  }
  for (int a=255; a>=16;a--){
    analogWrite(activeLed, a);
    delay(delaytime);
  }
}

void blinkBlueLED ()     
{    
  int delaytime = 32;
  analogWrite(greenLedPin, 0);
  analogWrite(redLedPin, 0);
  analogWrite(blueLedPin, 0);
  
  for (int a=0; a<=64;a++){
    analogWrite(blueLedPin, a);
    delay(delaytime);
  }
  for (int a=64; a>=0;a--){
    analogWrite(blueLedPin, a);
    delay(delaytime);
  }
}

//DEMO-Code
void demo(){
  
  // Check cigar
  lowerCigar();

  //Check Main- & Secondary indicators
  for(int i = 0;i<3;i++){
    demoLed(redLedPin);
    demoLed(leftEyePin);
    demoLed(rightEyePin);
    demoLed(blueLedPin);
    demoLed(leftEyePin);
    demoLed(rightEyePin);
    demoLed(greenLedPin);
    demoLed(leftEyePin);
    demoLed(rightEyePin);
  }

  //Check external relay (for rotating light)
  digitalWrite(relayPin, HIGH);
  delay(5000);
  digitalWrite(relayPin, LOW);

  //Check Main- & Secondary indicators (just for the show)
  for(int i = 0;i<3;i++){
    demoLed(redLedPin);
    demoLed(leftEyePin);
    demoLed(rightEyePin);
    demoLed(blueLedPin);
    demoLed(leftEyePin);
    demoLed(rightEyePin);
    demoLed(greenLedPin);
    demoLed(leftEyePin);
    demoLed(rightEyePin);
  }
  analogWrite(blueLedPin, 255);
  analogWrite(leftEyePin, 255);
  analogWrite(rightEyePin, 255);
  delay(2000);
  analogWrite(leftEyePin, 0);
  analogWrite(rightEyePin, 0);

  //Show pulsing blue LED to indicator completion of the Demo
  for(int j = 0; j < 3;j++){
    for (int a=255; a>=16;a--){
       analogWrite(blueLedPin, a);
       delay(4);
    }
    for (int a=16; a<=255;a++){
       analogWrite(blueLedPin, a);
       delay(4);
    }
  }
  analogWrite(blueLedPin, 0);
}

/**
 * Used in demo mode
 */
void demoLed(int led){
    analogWrite(led, 255);
    delay(150);
    analogWrite(led, 0);
}
