import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Parser {
	
	//Instantiate a String to hold the path to the maze
	String filePath;
	String[][] stringMaze;
	
	//Create a constructor for Parser class that sets the value of filePath
	Parser(String filepath) throws IOException{
		this.filePath = filepath;
		
		//Define a FileReader object from the filePath
		FileReader input = new FileReader(this.filePath);
		BufferedReader bufRead = new BufferedReader(input);
		
		//Instantiate a read variable to hold the values of the file during iteration
		String mazeLine;

		//Read the first line of the maze
		String firstLine = bufRead.readLine();
		String[] firstSplit = firstLine.split("+");
		int length = firstSplit.length-2;
		//Instantiate the stringMaze matrix based upon the length of the provided first line;
		stringMaze = new String[(length*2)-1][length];
		
		//For all lines in the maze file
		while ( (mazeLine = bufRead.readLine()) != null){
			//Test if this is a dividing line
			
			if (Character.toString(mazeLine.charAt(0)).equals("+")){
				String[] dividedNorth = mazeLine.split("+");
				
			}
			
		}
	}

}
