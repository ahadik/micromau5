int rightPin1 = 5; 
int rightPin2 = 6;
int leftPin1 = 9;
int leftPin2 = 10;

int perSpeed = 50;
int actSpeed = 0;



int time;

void setup()
{
  pinMode(rightPin1, OUTPUT);
  pinMode(rightPin2, OUTPUT);
  pinMode(leftPin1, OUTPUT);
  pinMode(leftPin2, OUTPUT);
}


void leftMotorForward(float perSpeed)
{
  actSpeed = (perSpeed/100)*255; //convert % max speed to value [0,255]
  analogWrite(leftPin1, actSpeed); //analogWrite(pin,value) with value range [0, 255]
  analogWrite(leftPin2, 0);
}

void leftMotorReverse(float perSpeed)
{
  actSpeed = perSpeed*255;
  analogWrite(leftPin1, actSpeed);
  analogWrite(leftPin2, 0);
}

void rightMotorForward(float perSpeed)
{
  actSpeed = (perSpeed/100)*255; //convert % max speed to value [0,255]
  analogWrite(rightPin1, actSpeed); //analogWrite(pin,value) with value range [0, 255]
  analogWrite(rightPin2, 0);
}

void rightMotorReverse(float perSpeed)
{
  actSpeed = perSpeed*255;
  analogWrite(rightPin2, actSpeed);
  analogWrite(rightPin1, 0);
}

void loop()
{
  rightMotorForward(75);
  leftMotorForward(75);
  delay(4000);
  rightMotorForward(0);
  leftMotorForward(0);
  delay(1000);
}
