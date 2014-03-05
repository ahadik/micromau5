/*
 * ReceiveMultipleFieldsBinaryToFile_P
 *
 * portIndex must be set to the port connected to the Arduino
 * based on ReceiveMultipleFieldsBinary, this version saves data to file
 * Press any key to stop logging and save file
 */

import processing.serial.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

PrintWriter output;
DateFormat fnameFormat= new SimpleDateFormat("yyMMdd_HHmm");
DateFormat  timeFormat = new SimpleDateFormat("hh:mm:ss");
String fileName;

Serial myPort;        // Create object from Serial class
char HEADER = 'H';
String inString;

int counter;
int lf = 10; // Linefeed in ASCII


void setup()
{
  // Open whatever serial port is connected to Arduino.
  //String portName = "/dev/tty.usbmodem411";
  String portName = Serial.list()[9];
  println(" Connecting to -> " + portName);
  myPort = new Serial(this, portName, 9600);
  Date now = new Date();
  fileName = fnameFormat.format(now);
  output = createWriter(fileName + ".txt"); // save the file in the sketch folder
}

void draw()
{
   delay(100);

  inString = myPort.readStringUntil(lf);
  
  output.println(inString);
  output.flush();

  counter = counter + 1;
  
  if (counter == 100) {     
    output.close();
    println(" Done with read " + fileName);
    exit();
  }
}




/*
void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}*/

void writeData(String temp) {
 if (output != null) {
  output.println(temp);
 }
}

// return the integer value from bytes received on the serial port (in low,highorder)
/*
int readArduinoInt()
{
  int val;      // Data received from the serial port

  val = myPort.read();          // read the least significant byte
  val =  myPort.read() * 256 + val; // add the most significant byte
  return val;
}*/
