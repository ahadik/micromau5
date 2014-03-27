//Include custom data types
#include "coord.h"

  //Define some global constants
  #define X 16
  #define Y 16
    
  int array[X];
  int maze[Y][X];
  int latentMaze[Y][X];
  //N,S,E,W
  int headings[] = {1,2,4,8};

void setup(){
}

//Get the most optimistic distance between two coordinates in a grid
int calcDist(int posx, int posy, int desireX, int desireY){
  int dist = abs(desireY-posy)+abs(desireX-posx);
  return dist;
}

//Get the most optimistic distance between a given coordinate and a 2x2 square in the center of a maze of dimension dim (dim must be even)
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

int orient(coord currCoord, int heading){
  
  coord leastNext = {0,0};
  //This is the absolute largest value possible (dimension of maze squared)
  int leastNextVal = sizeof(maze)*sizeof(maze);
  int leastDir = heading;
  
  //If there is a bitwise equivalence between the current heading and the cell's value, then the next cell is accessible
  if((maze[currCoord.x][currCoord.y] & heading) != 0){
    //Define a coordinate for the next cell based onthis heading and set the leastNextVal t its value
    coord leastnextTemp = bearingCoord
  }
  
  
}

void loop(){}

