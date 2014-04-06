//Include custom data types
#include "coord.h"

  //Define some global constants
  #define X 16
  #define Y 16
  #define xMid 8
  #define yMid 8
    
  int array[X];
  int maze[Y][X];
  int latentMaze[Y][X];
  int headings[] = {1,2,4,8};

void setup(){
}

int calcDist(int posx, int posy, int mazeInput[Y][X]){
  int dist = abs(yMid-Y)+abs(xMid-X);
  return dist;
}

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

int heading orient(coord currCoord, int heading){
  coord leastnext = null;
  double leastNextVal = null;
  int leastDir = heading;
  
  if (()!=0)
  
}



void loop(){}

