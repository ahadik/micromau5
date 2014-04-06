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
  if((maze[currCoord.x][currCoord.y] & heading) != 0){
    //Define a coordinate for the next cell based onthis heading and set the leastNextVal t its value
    coord leastnextTemp = bearingCoord(currCoord, heading);
    
    if(checkBounds(leastnextTemp)){
      leastNext = leastnextTemp;
      leastNextVal = latentMaze[leastNext.x][leastNext.y];
    }
  }
  
  for(int i=0; i<sizeof(headings); i++){
    int dir = headings[i];
    //if this dir is accessible
    if((maze[currCoord.x][currCoord.y] & dir) != 0){
      //define the coordiante for this dir
      coord dirCoord = bearingCoord(currCoord,dir);
      
      if(checkBounds(dirCoord)){
        //if this dir is more optimal than continuing straight
        if(latentMaze[dirCoord.x][dirCoord.y] < leastNextVal){
          //update teh value of leastNextVal
          leastNextVal = latentMaze[dirCoord.x][dirCoord.y];
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
    if((maze[Coord.x][Coord.y] & dir) != 0){
      //Get the coordinate of the accessible neighbor
      coord neighCoord = bearingCoord(Coord, dir);
      //Check the value of the accessible neighbor
      if (checkBounds(neighCoord)){
        //if the neighbore is less than the current recording minimum value, update the minimum value
        //If minVal is null, set it right away, otherwise test
        if(latentMaze[neighCoord.x][neighCoord.y] < minVal){minVal = latentMaze[neighCoord.x][neighCoord.y];}
      }
    }
  }
  return minVal;
}

//Given a coordinate, test and return if the coordinate is bounded on three sides
boolean isDead(coord coord){
  boolean deadEnd = false;
  if(checkBounds(coord)){
    int bounds = maze[coord.x][coord.y];
    //bounds is the integer from the exploratory maze that represents the known walls of the coordinate
    if((bounds == 1)||(bounds == 2)||(bounds == 4) || (bounds == 8)){deadEnd=true;}
  }
  return deadEnd;
}

void loop(){}
