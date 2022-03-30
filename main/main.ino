#include <EEPROM.h>
#include <QTRSensors.h>
#include <LiquidCrystal_I2C.h>

//PID Coefficients and Variables -------------------------------------------------------------------------------------------------------

float Kp; // experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
float Kd; // experiment to determine this, slowly increase the speeds and adjust this value. ( Note: Kp < Kd)
float Ki;

byte maxSpeed;
byte minSpeed;
byte baseSpeed;

int lastError = 0;
int integral=0;

//Variables for Left Hand Rule ----------------------------------------------------------------------------------------------------------

bool left=0, right=0, straight=0, dead=0, detect=0, Tjunction=0, End=0;
String turns="" , back_turns="";
byte index=0;

//Mototrs -------------------------------------------------------------------------------------------------------------------------------

#define leftMotorForward 9  //motor directions
#define leftMotorBackward 8
#define rightMotorForward 10
#define rightMotorBackward 11

#define rightMotorPWM 12 //PWM controls
#define leftMotorPWM 7

//Encoders ------------------------------------------------------------------------------------------------------------------------------

volatile unsigned long leftCount=0, rightCount=0;

void leftISR() //ISR for left motor count
{
  leftCount++;
}

void rightISR() //ISR for right motor count
{
  rightCount++;
}

//Buzzer --------------------------------------------------------------------------------------------------------------------------------

#define buzzer 6

//LCD Menu ------------------------------------------------------------------------------------------------------------------------------

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//#define leftPin 4
#define centerPin 3
#define rightPin 2

bool centerButton=0, rightButton=0;

//QTR Sensor ----------------------------------------------------------------------------------------------------------------------------

#define NUM_SENSORS  8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN   22

QTRSensors qtr;

unsigned int position;
unsigned int sensorValues[NUM_SENSORS];
bool dval[NUM_SENSORS];

//default values for robot motion -------------------------------------------------------------------------------------------------------
char manner='S';
byte brakeTime=40;

//Setup the bot -------------------------------------------------------------------------------------------------------------------------
void setup() 
{
  Serial.begin(9600);                     //Begin serial communication at 9600 baud rate
  interrupts();                           //turn on interrupts explicitly

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

  pinMode(33, OUTPUT); //Green            //RGB light (High Power Led)
  pinMode(35, OUTPUT); //Blue
  pinMode(37, OUTPUT); //Red
  pinMode(39, OUTPUT); //Red              //RGB light (Normal Led)
  pinMode(41, OUTPUT); //Green
  pinMode(43, OUTPUT); //Blue
  
  lcd.init();                             // initialize the lcd display
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  //pinMode(leftPin, INPUT_PULLUP);         //button for menu
  pinMode(centerPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  
  pinMode(buzzer, OUTPUT);                //pin for buzzer
  
  pinMode(leftMotorForward, OUTPUT);      //motor controlling pins
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);
  
  pinMode(18, INPUT_PULLUP);              //Interrupts assigning for Encoders
  pinMode(19, INPUT_PULLUP);
  attachInterrupt(4, leftISR, CHANGE); 
  attachInterrupt(5, rightISR, CHANGE);
  
  menu();   //begin menu
}

//Main Code -----------------------------------------------------------------------------------------------------------------------------

void loop()
{
  lcd.clear();      //interface for main code 
  lcd.print("Main Code...");
  lcd.setCursor(0,1);
  delay(100);
  attachInterrupt(0, reset, CHANGE);
  lcd.print("           RESET");

  qtrLoad();      //Loading calibrated sensor values
  
  //MAIN CODE ------------------------------------------------------

  solveMesh();
  beep(1000);
  light(' ');

  goHome();

  //End
  menu();
}
