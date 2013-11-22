FILE EXPORT FROM ARDUINO

OVERVIEW
Use this sample code to write data communicated over the Arduino Serial interface to a file on a computer. Good for keeping logs of Arduino output.

USE
Arduino
	In your Arduino script, use Serial.println("TEXT"); to send data over the Serial interface to the computer. Any form of data can be communicated.

Processing
	After uploading your script to the Arduino and opening the Com port, run the Processing script to begin recording data. Be sure to run the Arduino before you run the Processing script to avoid a port clash.

	Data is printed to file with the code:

		String inString = myPort.readString();
		output.println(inString);

	inString defines the String taken as input from the Arduino
	output.println(inString) prints the String variable to the output file.

OUTPUT
The output file takes the standard of creating a new file in the same directory as the processing script, and saving it with the current date and time to avoid overwriting files.