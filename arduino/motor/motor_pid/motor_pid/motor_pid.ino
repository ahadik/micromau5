#include "Encoder_Polling.h"
//#include <Encoder.h>

//#include <Wire.h>
//#include <L3G.h>

const int R_encoder_A = 2;
const int R_encoder_B = 11;
//const int L_encoder_A = 3;
//const int L_encoder_B  = 12;

const int R_motor_forward = 5;
const int R_motor_backward = 6;
//const int L_motor_forward = 9;
//const int L_motor_backward = 10;

/*
const int R_encoder_A = 2;
const int R_encoder_B = 6;
const int L_encoder_A = 5;
const int L_encoder_B = 12;

const int R_motor_forward = 3;
const int R_motor_backward = 11;
const int L_motor_forward = 9;
const int L_motor_backward = 10;
*/
int counter = 0;
int motorSpeed = 0;
int actSpeed = 0;

//L3G gyro;

// PID controls
int damping = 0.1;

//Encoder rightEnc(R_encoder_A, R_encoder_B);
//Encoder leftEnc(L_encoder_A, L_encoder_B);

//long positionRight = -999;
//long positionLeft = -999;

int oneTurn = 512;

void setup()
{
  pinMode(R_motor_forward, OUTPUT);
  pinMode(R_motor_backward, OUTPUT);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  
  //pinMode(L_motor_forward, OUTPUT);
 // pinMode(L_motor_backward, OUTPUT);
  pinMode(R_encoder_A, INPUT);
  pinMode(R_encoder_B, INPUT);
 // pinMode(L_encoder_A, INPUT);
 // pinMode(L_encoder_B, INPUT);
  
  
  // TCCR0B is pins 5 and 6
  // 0x01 = 62500
  // 0x02 = 7812.5
  //TCCR0B = TCCR0B & 0b11111000 | 0x01;
  
  // TCCR1B is pins 9 and 10
  // TCCR2B is pins 3 and 11
  //TCCR1B = TCCR1B & 0b11111000 | 0x01;
  //TCCR2B = TCCR2B & 0b11111000 | 0x01;
  
  Serial.begin(115200);
  Serial.println("Starting");
  encoder_begin(R_encoder_A, R_encoder_B);

 // Wire.begin();
  
  /*if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while(1); 
  }*/
  
  //gyro.enableDefault();
}


void motorMove(float perSpeed, int pin1, int pin2) {
   actSpeed = (perSpeed/100)*255;
   analogWrite(pin1, actSpeed);
   analogWrite(pin2, 0); 
}

  
void loop()
{
  //gyro.read();
  //Serial.println((int)gyro.g.z);
  
  /*
  long newRight; //, newLeft;
  newRight = rightEnc.read();
  if (newRight != positionRight) {
    positionRight = newRight;
  
 // newLeft = leftEnc.read();
 // Serial.println(newLeft);
 // Serial.println("hello");
      Serial.println(newRight);
  }
*/
//  motorMove(2.0, R_motor_forward, R_motor_backward);
    /*
  if (newLeft >= -10*oneTurn) {
    motorMove(50.0, L_motor_forward,L_motor_backward);
  }
  else {
    motorMove(0,L_motor_forward, L_motor_backward);
  }*/

  
  /*
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }*/
  
  motorMove(5.0, R_motor_forward,R_motor_backward);
 
  int dir = encoder_data(); // Check for rotation
   
  if(dir == 1)       // If its forward...
  {
    counter++;       // Increment the counter
    if (counter & 512)
    {
      counter = 0; 
    }
    Serial.println(counter);
  }
  else if(dir == -1) // If its backward...
  {
    counter--;       // Decrement the counter
    if (counter & -512)
    {
      counter = 0; 
    }
    Serial.println(counter);
  }
  
}
  
  
