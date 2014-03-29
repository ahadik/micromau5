#include <IR.h>

//Define integer variables for input and output pins.
const int ir_side_out = 4;
const int ir_front_out = 8;
int ir_diag_out = 7;


const int front_r_in = A7;//confirmed
const int front_l_in = A6;//confirmed
const int side_r_in = A3;
const int side_l_in = A2;//confirmed
const int diag_r_in = A1;//confirmed
const int diag_l_in = A0;//confirmed

IR ir(ir_diag_out, ir_side_out, ir_front_out, front_r_in, front_l_in, side_r_in, side_l_in, diag_r_in, diag_l_in);

void setup(){
  Serial.begin(9600);
  
  ir.calibrateSensors();
  
  pinMode(3,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(3,LOW);
  digitalWrite(11,LOW);
  
}

void loop(){

  
  for(int i=0; i<3; i++){
    ir.powerUp(ir.irArray[i][0]);
    //Serial.println(ir.irArray[i][1]);
    delayMicroseconds(80);
    for(int j=1; j<3; j++){
      ir.readSensor(i,j);

      if(ir.irValues[ir.mapCoords(i,j)] > ir.calibratedArray[ir.mapCoords(i,j)]+10){
        if(i==0){
          if(j=1){
            Serial.println("Front Right");
          }else if(j==2){
            Serial.println("Front Left");
          }
        }else if(i==1){
          if(j=1){
            Serial.println("Diagonal Right");
          }else if(j==2){
            Serial.println("Diagonal Left");
          }
        }else if(i==2){
          if(j=1){
            Serial.println("Side Right");
          }else if(j==2){
            Serial.println("Side Left");
          }
        }
      }

    }
    ir.powerDown(ir.irArray[i][0]);
  }
  
*/  
  
}
