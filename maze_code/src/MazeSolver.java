import java.util.HashMap;
import java.util.Stack;


public class MazeSolver {
	
	private static int x;
	private static int y;
	private static int xMid;
	private static int yMid;
	private static int[][] expMaze;
	private static int[][] distMaze;
	private static int[][] maze;
	static double inf = Double.POSITIVE_INFINITY;
 
	public MazeSolver(int x, int y, int[][] maze) {
		this.x = x;
		this.y = y;
		this.xMid = (int) Math.floor((double) this.x/2);
		this.yMid = (int) Math.floor((double) this.y/2);
		this.maze = maze;
		distMaze = new int[this.x][this.y];
		expMaze = new int[this.x][this.y];
	}


	public static int calcDist(int x, int y) {
		int dist = Math.abs(yMid - y)+Math.abs(xMid - x);
		return dist;
	}
	
	public static String distString(int dist){
		String distVal;
		
			if (dist > 99){
				distVal = Integer.toString(dist);
			}else{
				if (dist > 9){
					distVal = "0"+Integer.toString(dist);
				}else{
					distVal = "00"+Integer.toString(dist);
				}
			}
		return distVal;
	}
	
	public static String navString(int dist){
		String distVal;
		

				if (dist > 9){
					distVal = Integer.toString(dist);
				}else{
					distVal = "0"+Integer.toString(dist);
				}

		return distVal;
	}
	
	public static void track(Tuple<Integer,Integer> coord) {
		for (int i = 0; i < y; i++) {
			// draw the north edge
			for (int j = 0; j < x; j++) {
				System.out.print((expMaze[j][i] & 1) == 0 ? "+---" : "+   ");
			}
			System.out.println("+");
			// draw the west edge
			for (int j = 0; j < x; j++) {
				String dist = distString(distMaze[j][i]);
				if ((i==coord.y) && (j==coord.x)){
					System.out.print((expMaze[j][i] & 8) == 0 ? "| * ": "  * ");
				}else{
					
					System.out.print((expMaze[j][i] & 8) == 0 ? "|"+dist : " "+dist);
				}
			}
			System.out.println("|");
		}
		// draw the bottom line
		for (int j = 0; j < x; j++) {
			System.out.print("+---");
		}
		System.out.println("+");
	}
	
	public void dirmark() {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j< x; j++) {
				System.out.print((expMaze[j][i] & 1) == 0 ? "+---" : "+   ");
			}
			System.out.println("+");
			for (int j = 0; j < x; j++) {
				String coord = distString(expMaze[j][i]);
				System.out.print((expMaze[j][i] & 8) == 0 ? "|"+coord+" " : " "+coord+" ");
				distMaze[j][i] = calcDist(i,j);
			}
			System.out.println("|");
		}
		// draw the bottom line
		for (int j = 0; j < x; j++) {
			System.out.print("+---");
		}
		System.out.println("+");
	}
	
	public void distmark() {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j< x; j++) {
				if (i>0){
					System.out.print(((expMaze[j][i] & 1) == 0)||((expMaze[j][i-1] & 2)==0) ? "+---" : "+   ");
				}else{
					System.out.print((expMaze[j][i] & 1) == 0 ? "+---" : "+   ");
				}
			}
			System.out.println("+");
			for (int j = 0; j < x; j++) {
				String dist = distString(distMaze[j][i]);
				if (j>0){
					System.out.print(((expMaze[j][i] & 8) == 0)||((expMaze[j-1][i] & 4) == 0) ? "|"+dist : " "+dist);
				}else{
					System.out.print((expMaze[j][i] & 8) == 0 ? "|"+dist : " "+dist);
				}
				
				distMaze[j][i] = calcDist(i,j);
			}
			System.out.println("|");
		}
		// draw the bottom line
		for (int j = 0; j < x; j++) {
			System.out.print("+---");
		}
		System.out.println("+");
	}
	
	/*
	 * Input: A Tuple representing the current coordinate and an integer representing the robots current heading
	 * Output: A Tuple representing the next coordinate based upon that heading.
	 */
	public static Tuple<Integer, Integer> bearingCoord(Tuple<Integer, Integer> currCoord, Integer heading){
		
		Tuple<Integer,Integer> nextCoord = new Tuple<Integer,Integer>(0,0);
		
		switch(heading) {
		case 1: nextCoord.x=currCoord.x; nextCoord.y=currCoord.y-1; break;
		case 2: nextCoord.x=currCoord.x; nextCoord.y=currCoord.y+1; break;
		case 4: nextCoord.x=currCoord.x+1; nextCoord.y=currCoord.y; break;
		case 8: nextCoord.x=currCoord.x-1; nextCoord.y=currCoord.y; break;
		}
		return nextCoord;
	}

	/*
	 * Input: A Tuple representing the current coordinate and the robots current heading
	 * Output: A Tuple representing the next optimal coordinate for the robot
	 * 
	 * Notes: This function gives preference for the robot staying straight. 
	 * A coordinate other than the heading provided as an argument will be returned only
	 * if turning offers a more optimal score from the distance matrix.
	 */
	public static Tuple<Tuple<Integer,Integer>,Integer> orient(Tuple<Integer, Integer> currCoord, Integer heading){
		
		Tuple<Integer,Integer> leastnext = null;
		double leastNextVal = inf;
		int leastDir = heading;
		//If there is bitwise equivalence between the current heading and the cell's value, then the next cell is accessible
		if ((expMaze[currCoord.x][currCoord.y] & heading) != 0){
			
			//Define a coordinate for the next cell based on this heading and set the leastNextVal to its value
			Tuple<Integer,Integer> leastnextTemp = bearingCoord(currCoord,heading);
			if(checkBounds(leastnextTemp)){
				leastnext = leastnextTemp;
				leastNextVal = distMaze[leastnext.x][leastnext.y];
			}
		}
		//Define array of possible headings
		int[] headings = new int[]{1,2,4,8};
		
		/*For each of the possible headings, check if they are accessible, 
		 * and if their value is less than the currently set value. If both 
		 * are true, then set the next coord appropriately
		 */
		for(int dir : headings){
			//If this dir is accessible
			if ((expMaze[currCoord.x][currCoord.y] & dir) != 0){
				
				//Define the coordinate for this dir
				Tuple<Integer,Integer> dirCoord = bearingCoord(currCoord, dir);
				
				if(checkBounds(dirCoord)){
					//If this dir is more optimal than continuing straight
					if((double) distMaze[dirCoord.x][dirCoord.y] < leastNextVal){
						//Update the value of leastNext Val
						leastNextVal = (double) distMaze[dirCoord.x][dirCoord.y];
						//Update the value of least next to this dir.
						leastnext = dirCoord;
						leastDir = dir;
					}
				}
			}
			
		}
		return new Tuple<Tuple<Integer,Integer>,Integer>(leastnext,leastDir);
	}
	
	public static Boolean checkBounds(Tuple<Integer,Integer> coord){
		if ((coord.x >= x) || (coord.y >= y) || (coord.x < 0) || (coord.y < 0)){return false;}else{return true;}
	}
	
	/*
	 * Input: Tuple representing coordinates in the maze
	 * Output: A Tuple with a Boolean representing if the minimum coord accessible from the provided coord is one less than the provided coord 
	 */
	public static Tuple<Boolean, Integer> checkNeighs(Tuple<Integer,Integer> coord){
		boolean returnBool = false;
		
		//Define array of possible headings
		int[] headings = new int[]{1,2,4,8};
		double minVal = inf;
		Tuple<Integer,Integer> minCoord = null;
		for (int dir : headings){
			//Check that heading is accessible
			if ((expMaze[coord.x][coord.y] & dir) != 0){
				//Get the coordinate of the accessible neighbor
				Tuple<Integer,Integer> neighCoord = bearingCoord(coord, dir);
				//Check the value of the accessible neighbor
				if (checkBounds(neighCoord)){
					//If the neighbor is less than the current recorded minimum value, update the minimum value
					if ((double) distMaze[neighCoord.x][neighCoord.y] < minVal){minVal = (double) distMaze[neighCoord.x][neighCoord.y]; minCoord = neighCoord;}
				}
			}
		}
		//Check that the value of the coord argument is one greater than the minimum value
		if (((int) minVal<distMaze[coord.x][coord.y])||(isEnd(coord))){
		//if (((int) minVal+1)==distMaze[coord.x][coord.y]){
			returnBool = true;
		}
		
		return new Tuple<Boolean,Integer>(returnBool,(int) minVal);
	}
	
	/*
	 * Return a boolean indicating if the coordinate is an identified dead end
	 */
	public static Boolean isDead(Tuple<Integer,Integer> coord){
		Boolean deadEnd = false;
		if (checkBounds(coord)){
			int dist = expMaze[coord.x][coord.y];
			//If this coordinate represents an identified dead end, set return to TRUE
			if((dist == 1)||(dist == 2)||(dist == 4)||(dist == 8)){deadEnd = true;}
		}
		return deadEnd;
	}
	
	public static Boolean isEnd(Tuple<Integer,Integer> coord){
		Boolean End = false;
		if (checkBounds(coord)){
			//If this coordinate represents an identified dead end, set return to TRUE
			if((coord.x == xMid)&&(coord.y == yMid)){End = true;}
		}
		return End;
	}
	
	/*
	 * Input: A Tuple representing the robots current coordinate
	 * Output: Void
	 * Functionality: This function updates the maze to account for learned walls of the maze.
	 */
	public static void floodFillUpdate(Tuple<Integer,Integer> currCoord){
		Stack<Tuple<Integer,Integer>> entries = new Stack<Tuple<Integer,Integer>>();
		//Update the value of the exploratory maze to the value of the generated maze
		expMaze[currCoord.x][currCoord.y]=MazeSolver.maze[currCoord.x][currCoord.y];
		System.out.println("1");
		entries.push(currCoord);
		//Define array of possible headings
		int[] headings = new int[]{1,2,4,8};
		for (int dir : headings){
			//If there's a wall in this direction
			if ((expMaze[currCoord.x][currCoord.y] & dir) == 0){
				Tuple<Integer,Integer> workingCoord = new Tuple<Integer,Integer>(currCoord.x,currCoord.y);
				switch (dir){
					case 1: workingCoord.y=workingCoord.y-1; coordUpdate(workingCoord,2); break;
					case 2: workingCoord.y=workingCoord.y+1; coordUpdate(workingCoord,1); break;
					case 4: workingCoord.x=workingCoord.x+1; coordUpdate(workingCoord,8); break;
					case 8: workingCoord.x=workingCoord.x-1; coordUpdate(workingCoord,4); break;
				}
				//If the working coordinate has been identified as a dead end, add it to the stack for analysis.
				//if(isDead(workingCoord)){entries.push(workingCoord);}
				if(checkBounds(workingCoord)&&(!isEnd(workingCoord))){
				//	System.out.println("2");
					entries.push(workingCoord);
				}
			}
		}
		while(!entries.empty()){
			//Pop an entry from the stack
			Tuple<Integer,Integer> workingEntry = entries.pop();
			Tuple<Boolean,Integer> neighCheck = checkNeighs(workingEntry);
			if (!neighCheck.x){
				if(isEnd(workingEntry)){System.out.println("WTF");}
				distMaze[workingEntry.x][workingEntry.y] = neighCheck.y+1;
				for (int dir : headings){
					if ((expMaze[workingEntry.x][workingEntry.y] & dir) != 0){
						Tuple<Integer,Integer> nextCoord = bearingCoord(workingEntry,dir);
						if(checkBounds(nextCoord)){
							//Push the coordinate onto the entries stack
							//if(isEnd(nextCoord)){System.out.println("3");}
							if(!isEnd(nextCoord)){
								entries.push(nextCoord);
							}
						}
					}
				}
			}
		}
	}
	
	/*
	 * INPUT: A tuple representing the coordinate to update, and a direction representing the wall to add
	 * OUTPUT: Update to coordinate adding the wall provided as an argument
	 */
	public static void coordUpdate(Tuple<Integer,Integer> coordinate, Integer wallDir){
		if (checkBounds(coordinate)){
			if ((expMaze[coordinate.x][coordinate.y] & wallDir) != 0){
				expMaze[coordinate.x][coordinate.y] = expMaze[coordinate.x][coordinate.y]-wallDir;
			}
		}
	}
	
	public static void floodFill(){
		Tuple<Integer,Integer> currCoord = new Tuple<Integer,Integer>(0,0);
		int heading = 8;
		/*Integer representation of heading
		 * 1 = N
		 * 4 = E
		 * 2 = S
		 * 8 = W
		 */
		while(distMaze[currCoord.x][currCoord.y] != 0){
			floodFillUpdate(currCoord);
			Tuple<Tuple<Integer,Integer>,Integer> nextHeading = orient(currCoord, heading);
			Tuple<Integer,Integer> nextCoord = nextHeading.x;
			int nextDir = nextHeading.y;
			currCoord = nextCoord;
			heading = nextDir;
			track(currCoord);
			System.out.println();
		}
	}
	
	/*
	 * Function: Fill the distMaze in with most optimistic values
	 */
	public void instantiate() {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				String dist = distString(calcDist(i,j));
				distMaze[j][i] = calcDist(i,j);
				expMaze[j][i] = 15;
				//If this is the left column (0,x)
				if(j==0){
					expMaze[j][i] = 7;
				}
				if(i==0){
					expMaze[j][i] = 14;
				}
				if(j==(x-1)){
					expMaze[j][i] = 11;
				}
				if(i==(y-1)){
					expMaze[j][i] = 13;
				}
				expMaze[0][0] = 6;
				expMaze[x-1][0] = 10;
				expMaze[0][y-1] = 5;
				expMaze[x-1][y-1] = 9;
			}
		}
	}

	public static void main(String[] args) {
		int x = args.length >= 1 ? (Integer.parseInt(args[0])) : 8;
		int y = args.length == 2 ? (Integer.parseInt(args[1])) : 8;
		
		//Generate a random maze for testing
		MazeGenerator maze = new MazeGenerator(x, y);
		//Generate a blank maze for exploring
		MazeSolver distMaze = new MazeSolver(x, y, maze.maze);
		maze.display();
		distMaze.instantiate();
		floodFill();
		distMaze.distmark();
	}
	
}
