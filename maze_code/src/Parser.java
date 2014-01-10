import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Parser {
	
	//Instantiate a String to hold the path to the maze
	String filePath;
	String[][] stringMaze;
	int index = 0;
	
	/*
	 * INPUT: A string of spaces
	 * OUTPUT: An integer representing the number of cells those spaces represent
	 */
	public Integer spreadAmount(String spread){
		int spreadLength = spread.length()+1;
		int numCells = spreadLength/4;
		return numCells;
	}
	
	public void populateDashedLine(){
		for(int i = 1;i<midString.length-1;i++){
			
		}
	}
	
	//Create a constructor for Parser class that sets the value of filePath
	Parser(String filepath) throws IOException{
		this.filePath = filepath;
		
		//Define a FileReader object from the filePath
		FileReader input = new FileReader(this.filePath);
		BufferedReader bufRead = new BufferedReader(input);
		
		//Instantiate a read variable to hold the values of the file during iteration
		String primarymazeLine = bufRead.readLine();
		String[] primarySplit = primarymazeLine.split("+");
		int length = primarySplit.length-2;
		//Instantiate the stringMaze matrix based upon the length of the provided first line;
		stringMaze = new String[(length*2)+1][(length*2)+1];
		
		
		
		//Read in the first two lines to initialize them
		String firstLine = bufRead.readLine();;
		String secondLine = bufRead.readLine();;
		String[] firstSplit = firstLine.split("|");;
		String[] secondSplit = secondLine.split("+");;
		
		//For all lines in the maze file
		while (firstLine != null){
			
			
			
			//Read the first line of the maze
			firstLine = bufRead.readLine();
			secondLine = bufRead.readLine();
			firstSplit = firstLine.split("|");
			secondSplit = secondLine.split("+");
			//Update the index by one
			index++;
			
		}
	}

}
