//Libraries --------------------------------------
#include "BluetoothSerial.h" 

//BluetoothSerial-Objekt
BluetoothSerial SerialBT;	

//Define Motors ----------------------------------
//Xmotor (solenoid)   
const int pulsPinX(26); // analog pin of puls/step pin
const int dirPinX(27);  // analog pin of direction pin
const int enPinX(25);   // analog pin of enable pin

//Ymotor (rolls)
const int pulsPinY(23); // analog pin of puls/step pin
const int dirPinY(22);  // analog pin of direction pin
const int enPinY(21);   // analog pin of enable pin

//Safety switch ----------------------------------
const int switchPin(35); // analog pin of switch

//Solenoid ---------------------------------------
const int solenPin(14); // analog pin of solenoid

//Piezo Buzzer -----------------------------------
const int piezoPin(32); // analog pin of piezo

//Configure Braille Measurements
//Scope Pulley = 12,22mm*pi = 38,37mm
//Linear distance/step = 38,37mm/(360°/0,9°) = 0,11mm/step
//Step count = 2,5mm/0,11mm/step = 23,5 = 24 steps
//Step count = 3,5mm/0,11/step = 32,94 = 33 steps
const int startX = 10;
const int nextDot = 24;
const int nextCell = 33;
const int nextLine = 143;

//User input --------------------------------------
const byte numChars = 100;
char inputCharArray[numChars];  // array to store received data
boolean newData = false;
int inputLength = 0;

void setup() {
  //bluetooth setup
  Serial.begin(9600);  //baud rate
  SerialBT.begin("BrailleDrucker"); //device name for identification
  Serial.println("Braille Drucker ist bereit zum Verbinden.");

  //Initialize ESP32 pins
  pinMode(pulsPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(enPinX, OUTPUT);
  pinMode(pulsPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(enPinY, OUTPUT);
  pinMode(switchPin, INPUT);  
  pinMode(solenPin, OUTPUT); 
  pinMode(piezoPin, OUTPUT); 
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
}