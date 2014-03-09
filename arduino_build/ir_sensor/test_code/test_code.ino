//Define integer variables for input and output pins.
ir_diag_out = 7;
ir_side_out = 4;
ir_front_out = 8;
front_r_in = A0;
front_l_in = A1;
side_r_in = A2;
side_l_in = A3;
diag_r_in = A6;
diag_l_in = A7;

void setup()
{
  //Set intput and output modes for defined pins
    Serial.begin(9600);  //Begin serial communcation
    pinMode( ir_diag_out, OUTPUT );
    pinMode( ir_side_out, OUTPUT );
    pinMode( ir_front_out, OUTPUT );
 
    pinMode( front_r_in, INPUT );
    pinMode( front_l_in, INPUT );
    pinMode( side_r_in, INPUT );
    pinMode( side_l_in, INPUT );
    pinMode( diag_r_in, INPUT );
    pinMode( diag_l_in, INPUT );
}

int readSensor(int ledPin, int recPin)
{
  digitalWrite(ledPin, HIGH);
  delay(.08);
  
}

void loop()
{
//    Serial.println(analogRead(lightPin)); //Write the value of the photoresistor to the serial monitor.
//    analogWrite(ledPin, 255);  //send the value to the ledPin. Depending on value of resistor 
                                                //you have  to divide the value. for example, 
                                                //with a 10k resistor divide the value by 2, for 100k resistor divide by 4.
//   delay(80); //short delay for faster response to light.
}
