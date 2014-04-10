#include <Encoder.h>
#include <Dispatch.h>
#include <StackList.h>
#include "Wire.h"
#include "L3G.h"

//Include custom data types
#include "coord.h"
#include "entry.h"

//Encoder Variables
const int R_encoder_A = 2;
const int R_encoder_B = 11;
const int L_encoder_A = 3;
const int L_encoder_B  = 12;

const int R_motor_forward = 5;
const int R_motor_backward = 9;
const int L_motor_forward = 6;
const int L_motor_backward = 10;
//Encoder instantiation
Encoder rightEnc(R_encoder_A, R_encoder_B);
Encoder leftEnc(L_encoder_A, L_encoder_B);
long positionRight = 0;
long positionLeft = 0;
float oneTurn = 2048.0;

int motorSpeed = 0;
int actSpeed = 0;

// PID controls
int damping = 0.1;



//IR Variable Setup
const int ir_side_out = 4;
const int ir_front_out = 8;
int ir_diag_out = 7;


const int front_r_in = A7;//confirmed
const int front_l_in = A6;//confirmed
const int side_r_in = A3;
const int side_l_in = A2;//confirmed
const int diag_r_in = A1;//confirmed
const int diag_l_in = A0;//confirmed


Dispatch dispatch(ir_diag_out, ir_side_out, ir_front_out, front_r_in, front_l_in, side_r_in, side_l_in, diag_r_in, diag_l_in);

  //Gyro shit
  L3G gyro;
  
  //Define some global constants
  #define X 16
  #define Y 16
    
  entry maze[Y][X];
  //N,S,E,W
  int headings[] = {1,2,4,8};
  unsigned long time; 
  int sampleTime=10; 
  int rate;
  
  int sampleNum=750;
    
  int dc_offset=0; 
  double noise=0;
  int prev_rate=0; 
  double angle=0; 


void setup(){
  Serial.begin(9600);
   dispatch.calibrateSensors();
  
  //Set IO for motors
  pinMode(R_encoder_A, INPUT);
  digitalWrite(R_encoder_A, LOW);
  pinMode(R_encoder_B, INPUT);
  digitalWrite(R_encoder_B, LOW);
  pinMode(L_encoder_A, INPUT);
  digitalWrite(L_encoder_A, LOW);
  pinMode(L_encoder_B, INPUT);
  digitalWrite(L_encoder_B, LOW);
  pinMode(R_motor_forward, OUTPUT);
  digitalWrite(R_motor_forward, LOW);
  pinMode(R_motor_backward, OUTPUT);
  digitalWrite(R_motor_backward, LOW);
  pinMode(L_motor_forward, OUTPUT);
  digitalWrite(L_motor_forward, LOW);
  pinMode(L_motor_backward, OUTPUT);
  digitalWrite(L_motor_backward, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(12, LOW);
  
 
  Wire.begin();
  
  while (!gyro.init());
  gyro.enableDefault();
  
  //Calculate initial DC offset and noise level
  for(int n=0;n<sampleNum;n++){ 
   gyro.read(); 
   dc_offset+=(int)gyro.g.z; 
  } 
  dc_offset=dc_offset/sampleNum; 
  
  for(int n=0;n<sampleNum;n++){ 
   gyro.read(); 

   if((int)gyro.g.z-dc_offset>noise) 
     noise=(int)gyro.g.z-dc_offset; 
    else if((int)gyro.g.z-dc_offset<-noise) 
     noise=-(int)gyro.g.z-dc_offset; 
  } 
    noise=noise/100; //gyro returns hundredths of degrees/sec 

  //print dc offset and noise level 
  Serial.println(); 
  Serial.print("DC Offset: "); 
  Serial.print(dc_offset); 
  Serial.print("\tNoise Level: "); 
  Serial.print(noise); 
  Serial.println(); 
  
  delay(7000);
 
  
 //moveDist(7.74);
}

void instantiate(){
  for(int j = 0; j<Y; j++){
    for(int i = 0; i<X; i++){
      maze[j][i].distance = calcCenter(i, j, X);
      maze[j][i].walls = 15;
      //If this is the left column (0,x)
      if(i==0){
        maze[j][i].walls = 7;
      }
      //if this is the top row
      if(j==0){
        maze[j][i].walls = 14;
      }
      //if this is the bottom row
      if(j==(Y-1)){
        maze[j][i].walls = 13;
      }
      //If this is the righ column
      if(i==(X-1)){
        maze[j][i].walls = 11;
      }
      maze[0][0].walls = 6;
      maze[Y-1][0].walls = 5;
      maze[0][X-1].walls = 10;
      maze[X-1][Y-1].walls = 9;
    }
  }
}
//Get the most optimistic distance between two coordinates in a grid
int calcDist(int posx, int posy, int desireX, int desireY){
  int dist = abs(desireY-posy)+abs(desireX-posx);
  return dist;
}

//Get the most optimistic distance between a given coordinate and a 
//2x2 square in the center of a maze of dimension dim (dim must be even)
int calcCenter(int posx, int posy, int dim){
  int center = dim/2;
  int dist = 0;
  
  if(posy<center){
    if(posx<center){
      //You're in the top left of the maze
      dist=calcDist(posx, posy, (center-1), (center-1));
    }else{
      //You're in the bottom left of the maze
      dist=calcDist(posx,posy, (center-1),center);
    }
  }else{
    if(posx>=center){
      //You're in the bottom right of the maze
      dist=calcDist(posx,posy,center,center);
    }else{
      //You're in the top right of the maze
      dist=calcDist(posx,posy,center,(center-1));
    }
  }
return dist;
}

/*
INPUT: a coordinate representing a current position, and a heading
OUTPUT: the coordinates of the next desired position based on the heading and current position
*/
coord bearingCoord(coord currCoord, int heading){
  coord nextCoord = {0,0};
  switch (heading){
    case 1:
      //code
      nextCoord.x=currCoord.x;
      nextCoord.y=currCoord.y-1;
      break;
    case 2:
      nextCoord.x=currCoord.x;
      nextCoord.y=currCoord.y+1;
      break;
    case 4:
      nextCoord.x=currCoord.x+1;
      nextCoord.y=currCoord.y;
      break;
    case 8:
      nextCoord.x=currCoord.x-1;
      nextCoord.y=currCoord.y;
      break;
  }
  return nextCoord;
}

/*
INPUT: A Coord representing the current coordiante and the robots current heading
OUTPUT: An optimal direction away from the current coordinate.
*/
int orient(coord currCoord, int heading){
  
  coord leastNext = {0,0};
  //This is the absolute largest value possible (dimension of maze squared)
  int leastNextVal = sizeof(maze)*sizeof(maze);
  int leastDir = heading;
  
  //If there is a bitwise equivalence between the current heading and the cell's value, then the next cell is accessible
  if((maze[currCoord.x][currCoord.y].walls & heading) != 0){
    //Define a coordinate for the next cell based onthis heading and set the leastNextVal t its value
    coord leastnextTemp = bearingCoord(currCoord, heading);
    
    if(checkBounds(leastnextTemp)){
      leastNext = leastnextTemp;
      leastNextVal = maze[leastNext.y][leastNext.x].distance;
    }
  }
  
  for(int i=0; i<sizeof(headings); i++){
    int dir = headings[i];
    //if this dir is accessible
    if((maze[currCoord.y][currCoord.x].walls & dir) != 0){
      //define the coordiante for this dir
      coord dirCoord = bearingCoord(currCoord,dir);
      
      if(checkBounds(dirCoord)){
        //if this dir is more optimal than continuing straight
        if(maze[dirCoord.y][dirCoord.x].distance < leastNextVal){
          //update teh value of leastNextVal
          leastNextVal = maze[dirCoord.y][dirCoord.x].distance;
          //update the value of leastnext to this dir
          leastNext = dirCoord;
          leastDir = dir;
        }
      }
    }
  }
  return leastDir;
}

//Take a coordinate and test if it is within the allowable bounds
boolean checkBounds(coord Coord){
  if((Coord.x >= X) || (Coord.y >= Y) || (Coord.x < 0) || (Coord.y < 0)){return false;}else{return true;}
}

/*
INPUT: Coord
OUTPUT: An integer that is the least neighbor
*/
int checkNeighs(coord Coord){
  int minVal =  sizeof(maze)*sizeof(maze);
  for(int i=0; i<sizeof(headings); i++){
    int dir = headings[i];
    //if this dir is accessible
    if((maze[Coord.y][Coord.x].walls & dir) != 0){
      //Get the coordinate of the accessible neighbor
      coord neighCoord = bearingCoord(Coord, dir);
      //Check the value of the accessible neighbor
      if (checkBounds(neighCoord)){
        //if the neighbore is less than the current recording minimum value, update the minimum value
        //If minVal is null, set it right away, otherwise test
        if(maze[neighCoord.y][neighCoord.x].distance < minVal){minVal = maze[neighCoord.y][neighCoord.x].distance;}
      }
    }
  }
  return minVal;
}

//Given a coordinate, test and return if the coordinate is bounded on three sides
boolean isDead(coord coord){
  boolean deadEnd = false;
  if(checkBounds(coord)){
    int bounds = maze[coord.y][coord.x].walls;
    //bounds is the integer from the exploratory maze that represents the known walls of the coordinate
    if((bounds == 1)||(bounds == 2)||(bounds == 4) || (bounds == 8)){deadEnd=true;}
  }
  return deadEnd;
}

boolean isEnd(coord Coord, coord DesiredArray[]){
  boolean End = false;
  for(int i=0; i<sizeof(DesiredArray);i++){
    coord Desired = DesiredArray[i];
    if(checkBounds(Coord)){
      if((Coord.x == Desired.x)&&(Coord.y==Desired.y)){
        End = true;
      }
    }
  }
  return End;
}

/*
This function makes calls to the dispatcher to get the following info
  -orientation
  -surrounding walls
Using orientation and walls, this information is mapped to a map integer in the global coordinate frame
*/
int readCurrent(){
  //TODO: Fill this bitch out
  return 0;
}

int readAhead(){
  return 0;
}
  

/*
INPUT: Coordindate to update, and a direction representing the wall to add
OUTPUT: Update to coordinate adding the wall provided as an argument
*/

void coordUpdate(coord coordinate, int wallDir){
  if(checkBounds(coordinate)){
    if((maze[coordinate.y][coordinate.x].walls & wallDir) != 0){
      maze[coordinate.y][coordinate.x].walls = maze[coordinate.y][coordinate.x].walls-wallDir;
    }
  }
}

/*
INPUT: Current Robot coordinate
OUTPUT: Update maze for learned walls
*/
void floodFillUpdate(coord currCoord, coord desired[]){
  StackList<coord> entries;
  
  maze[currCoord.y][currCoord.x].walls=readCurrent();
  entries.push(currCoord);
  
  for(int i=0; i<sizeof(headings); i++){
    int dir = headings[i];
    //If there's a wall in this dir
    if((maze[currCoord.y][currCoord.x].walls & dir) == 0){
      coord workingCoord = {currCoord.x,currCoord.y};
      switch(dir){
        case 1:
          workingCoord.y=workingCoord.y-1;
          coordUpdate(workingCoord,2);
          break;
        case 2:
          workingCoord.y=workingCoord.y+1;
          coordUpdate(workingCoord,1);
          break;
        case 4:
          workingCoord.x=workingCoord.x+1;
          coordUpdate(workingCoord, 8);
          break;
       case 8:
         workingCoord.x=workingCoord.x-1;
         coordUpdate(workingCoord,4);
         break;
      }
      //If the workingEntry is a valid entry and not a dead end, push it onto the stack
      if(checkBounds(workingCoord)&&(!isEnd(workingCoord, desired))){
        entries.push(workingCoord);
      }
    }
  }
  //While the entries stack isn't empty
  while(!entries.isEmpty()){
    //Pop an entry from the stack
    coord workingEntry = entries.pop();
    int neighCheck = checkNeighs(workingEntry);
    //If the least neighbor of the working entry is not one less than the value of the working entry
    if(neighCheck+1!=maze[workingEntry.y][workingEntry.x].distance){
      maze[workingEntry.y][workingEntry.x].distance=neighCheck+1;
      for(int i=0;i<sizeof(headings);i++){
        int dir = headings[i];
        if((maze[workingEntry.y][workingEntry.x].walls & dir) != 0){
          coord nextCoord = bearingCoord(workingEntry,dir);
          if(checkBounds(nextCoord)){
            if(!isEnd(nextCoord, desired)){
              entries.push(nextCoord);
            }
          }
        }
      }
    }
  }
}

void floodFill(coord desired[]){
  coord currCoord = {0,0};
  int heading = 4;
  /*Integer representation of heading
  * 1 = N
  * 4 = E
  * 2 = S
  * 8 = W
  */
  while(maze[currCoord.y][currCoord.x].distance != 0){
      floodFillUpdate(currCoord, desired);
      int nextHeading = orient(currCoord, heading);
      coord nextCoord = bearingCoord(currCoord, nextHeading);
      //TODO: ADD MOVING INSTRUCTIONS HERE
      //This should occur as a callback of the moving finishing
      currCoord = nextCoord;
      heading = nextHeading;
  }
}

void moveDist(float dist){
  //diam is 2.32833 cm
  //rad = 1.2319
  //circ = 2*pi*r = 7.740
  
  //small = 12
  //big = 40
  float circ = 7.74;
  int encoder_count = (int)((dist/circ)*(40.0/12.0)*oneTurn);

  float motorSpeed = .1;
  
  long currCountRight = rightEnc.read();
  long currCountLeft = leftEnc.read();

  int desiredCount = (int)currCountRight+encoder_count;
  
  float error=(desiredCount-abs(positionRight));
  
  while(((error<-150)||(error>150))){
    
    float damping = 1.0;
    
    error=(desiredCount-abs(positionRight));
    
    long newRight = rightEnc.read();
    positionRight = newRight;
//    Serial.println(newRight);

    positionRight = newRight;
   currCountRight = newRight;
//   Serial.println(desiredCount);
//   Serial.println((damping*error/desiredCount)*10.0);
//   Serial.println();
   
   //if the desired count is greater than the position
   if(error>0){
     motorMove((damping*error/desiredCount)*25.0, R_motor_forward,R_motor_backward);
     motorMove((damping*error/desiredCount)*25.0, L_motor_forward,L_motor_backward);
   }else{
     motorMove((damping*error/desiredCount)*25.0,R_motor_backward, R_motor_forward);
     motorMove((damping*error/desiredCount)*25.0, L_motor_backward, L_motor_forward);
   }
  }

  motorMove(0.0, R_motor_forward,R_motor_backward);
  motorMove(0.0, L_motor_forward,L_motor_backward);
}

void motorMove(float perSpeed, int pin1, int pin2) {
  if(perSpeed==0.0){
   actSpeed = (perSpeed/100)*255;
   analogWrite(pin1, actSpeed);
   analogWrite(pin2, 0); 
  }else if(perSpeed<3.0){perSpeed=3.0;}
   actSpeed = (perSpeed/100)*255;
   analogWrite(pin1, actSpeed);
   analogWrite(pin2, 0); 
}

void loop(){
  
  // Every 10 ms take a sample from the gyro 
  if(millis() - time > sampleTime) {
    time = millis(); // update the time to get the next sample 
    gyro.read(); 
    rate=((int)gyro.g.z-dc_offset)/50; 
    
    angle += ((double)(prev_rate + rate) * sampleTime) / 2000; 
    // remember the current speed for the next loop rate integration. 
    prev_rate = rate; 
    // Keep our angle between 0-359 degrees 
    
   if (angle < 0)
     angle += 360; 
   else if (angle >= 360)
     angle -= 360; 

    Serial.print("Angle: ");
    Serial.print(angle);    
    Serial.print("\trate: "); 
    Serial.println(rate); 
  }

 // dispatch.updateGyro();

  for(int i=0; i<3; i++){
    dispatch.powerUp(dispatch.irArray[i][0]);
    
    delayMicroseconds(80);
    for(int j=1; j<3; j++){
      dispatch.readSensor(i,j);
      /*

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
      */
    }
    
    dispatch.powerDown(dispatch.irArray[i][0]);
  }

  //Serial.println(dispatch.irValues[3]);
  
  gyro.read();
/*
  Serial.print("G ");
  Serial.print("X: ");
  Serial.println((int)gyro.g.x);
*/
  delay(100);
  
/*
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y);
  Serial.print(" Z: ");
  Serial.println((int)gyro.g.z);
  */
  
  
  coord desired[] = {{X-1,Y-1},{X-1,Y},{X,Y-1},{X,Y}};
  floodFill(desired);
  coord returnCoord[] = {{0,0}};
  floodFill(returnCoord);
  
}
