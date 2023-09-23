//Libraries --------------------------------------
#include <Wire.h>
//#include <AccelStepper.h>
//#include <MultiStepper.h>
#include "BluetoothSerial.h"  //stellt Kommunikation über Bluetooth bereit
BluetoothSerial SerialBT;     //BluetoothSerial-Objekt mit Namen SerialBT erstellen

//Define Motors ----------------------------------

//Xmotor
const int pulsPinX = 26;  // pulse/step from PWM signal
const int dirPinX = 27; // direction Pin
const int enPinX = 25;  // enable

//Ymotor
const int pulsPinY = 23; // pulse/step from PWM signal
const int dirPinY = 22;// direction Pin
const int enPinY = 21;  // enable

//Safety switch ----------------------------------
const int switchPin = 35;

//Solenoid ---------------------------------------
#define embossDuration = 20;
#define embossDelay = 50;
const int solenPin = 14;  // analog pin of solenoid

//Piezo Buzzer -----------------------------------
const int piezoPin = 32;  // Piezo

//Configure Braille Measurements
const int startX = 10;
const int nextDot = 20;
const int nextCell = 40;
const int nextLine = 100;


//User input --------------------------------------
String voice;  //to store the command
char c;       //to get characters from the command
const byte numChars = 100;
char inputCharArray[numChars];  // array to store received data
boolean newData = false;
int inputLength = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialBT.begin(9600); // Serial of bluetooth

  //Initialize Arduino pins
  pinMode(pulsPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(enPinX, OUTPUT);
  pinMode(pulsPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(enPinY, OUTPUT);
  pinMode(switchPin, INPUT);  // safety switch
  pinMode(solenPin, OUTPUT);  // output signal to solenoid for punching
  pinMode(piezoPin, OUTPUT);  // outpunt signal to piezo buzzer

}

void loop() {
  

  //Initialize motors
  turnOffSteppers(); // start with motors off
  
  // Move to Startposition (righat hand sidde of printer)
  Serial.print("Moving to default position... \n");
  startPosition();
 
  delay(2000);
  Serial.print("Motors in start position \n");
  Serial.print("Ready for input \n");

  // acoustic signal for input being ready
  tone(piezoPin, 100);
  delay(1000);
  noTone(piezoPin);


  // take user input from AMR Voice App
  takeUserInput();
  // show User input in Serial Monitor
  showUserInput();
  showLengthOfArray();

  
  // acoustic signal for starting to print
  startSignal();

  // start the printing process
  turnOnSteppers();
  firstCell();
  printBrailleChar();
  

}//end loop
