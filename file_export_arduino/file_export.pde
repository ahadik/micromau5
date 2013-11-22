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
short portIndex = 0;  // select the com port, 0 is the first port
char HEADER = 'H';

void setup()
{
  size(200, 200);
  // Open whatever serial port is connected to Arduino.
  String portName = "/dev/tty.usbmodemfa131";
  println(Serial.list());
  println(" Connecting to -> " + Serial.list()[portIndex]);
  myPort = new Serial(this, portName, 9600);
  Date now = new Date();
  fileName = fnameFormat.format(now);
  output = createWriter(fileName + ".txt"); // save the file in the sketch folder
}

void draw()
{
  int val;
  String time;

  String inString = myPort.readString();
  
  output.println(inString);
}

void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}

// return the integer value from bytes received on the serial port (in low,highorder)
int readArduinoInt()
{
  int val;      // Data received from the serial port

  val = myPort.read();          // read the least significant byte
  val =  myPort.read() * 256 + val; // add the most significant byte
  return val;
}
