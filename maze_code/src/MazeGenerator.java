
 
import java.util.Collections;
import java.util.Arrays;
import java.util.Stack;

 
/*
 * recursive backtracking algorithm
 * shamelessly borrowed from the ruby at
 * http://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking
 */ 

public class MazeGenerator {
	private final int x;
	private final int y;
	private final int xMid;
	private final int yMid;
	private final int[][] maze;
	private final int[][] distMaze;
	double inf = Double.POSITIVE_INFINITY;
 
	public MazeGenerator(int x, int y) {
		this.x = x;
		this.y = y;
		this.xMid = (int) Math.floor((double) this.x/2);
		this.yMid = (int) Math.floor((double) this.y/2);
		maze = new int[this.x][this.y];
		distMaze = new int[this.x][this.y];
		generateMaze(0, 0);
	}
 
	public int calcDist(int x, int y) {
		int dist = Math.abs(yMid - y)+Math.abs(xMid - x);
		return dist;
	}
	
	public String distString(int dist){
		String distVal;
		if (dist > 9){
			distVal = Integer.toString(dist);
		}else{
			distVal = "0"+Integer.toString(dist);
		}
		return distVal;
	}
	
	public void display() {
		for (int i = 0; i < y; i++) {
			// draw the north edge
			for (int j = 0; j < x; j++) {
				System.out.print((maze[j][i] & 1) == 0 ? "+---" : "+   ");
			}
			System.out.println("+");
			// draw the west edge
			for (int j = 0; j < x; j++) {
				System.out.print((maze[j][i] & 8) == 0 ? "|   " : "    ");
			}
			System.out.println("|");
		}
		// draw the bottom line
		for (int j = 0; j < x; j++) {
			System.out.print("+---");
		}
		System.out.println("+");
	}
 
	public void mark() {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j< x; j++) {
				System.out.print((maze[j][i] & 1) == 0 ? "+---" : "+   ");
			}
			System.out.println("+");
			for (int j = 0; j < x; j++) {
				String dist = distString(calcDist(i,j));
				System.out.print((maze[j][i] & 8) == 0 ? "|"+dist+" " : " "+dist+" ");
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
	
	public void dirmark() {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j< x; j++) {
				System.out.print((maze[j][i] & 1) == 0 ? "+---" : "+   ");
			}
			System.out.println("+");
			for (int j = 0; j < x; j++) {
				String coord = distString(maze[j][i]);
				System.out.print((maze[j][i] & 8) == 0 ? "|"+coord+" " : " "+coord+" ");
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
	
	public Tuple<Integer, Integer> bearingCoord(Tuple<Integer, Integer> currCoord, Integer heading){
		
		Tuple<Integer,Integer> nextCoord = new Tuple<Integer,Integer>(0,0);
		
		switch(heading) {
		case 1: nextCoord.x = currCoord.x; nextCoord.y=currCoord.y+1;
		case 2: nextCoord.x=currCoord.x; nextCoord.y=currCoord.y-1;
		case 4: nextCoord.x=currCoord.x+1; nextCoord.y=currCoord.y;
		case 8: nextCoord.x=currCoord.x-1; nextCoord.y=currCoord.y;
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
	
	public Tuple<Integer,Integer> orient(Tuple<Integer, Integer> currCoord, Integer heading){
		
		//We want to choose the orientation that leads to the least accessible value
		
		Integer currHeading = heading;
		Tuple<Integer,Integer> leastnext = null;
		double leastNextVal = inf;
		//If there is bitwise equivalence between the current heading and the cell's value, then the next cell is accessible
		if ((maze[currCoord.x][currCoord.y] & heading) == 1){
			//Define a coordinate for the next cell based on this heading and set the leastNextVal to its value
			leastnext = bearingCoord(currCoord, heading);
			leastNextVal = distMaze[leastnext.x][leastnext.y];
		}
		//Define array of possible headings
		int[] headings = new int[]{1,2,4,8};
		
		/*For each of the possible headings, check if they are accessible, 
		 * and if their value is less than the currently set value. If both 
		 * are true, then set the next coord appropriately
		 */
		for(int dir : headings){
			//If this dir is accessible
			if ((maze[currCoord.x][currCoord.y] & dir) == 1){
				
				//Define the coordinate for this dir
				Tuple<Integer,Integer> dirCoord = bearingCoord(currCoord, dir);
				
				//If this dir is more optimal than continuing straight
				if((double) distMaze[dirCoord.x][dirCoord.y] < leastNextVal){
					//Update the value of leastNext Val
					leastNextVal = (double) distMaze[dirCoord.x][dirCoord.y];
					//Update the value of least next to this dir.
					leastnext = dirCoord;
				};
			}
			
		}
		
		return leastnext;
	}
	
	public boolean checkNeighs(Tuple<Integer,Integer> coord){
		boolean returnBool = false;
		
		//Define array of possible headings
		int[] headings = new int[]{1,2,4,8};
		double minVal = inf;
		for (int dir : headings){
			//Check that heading is accessible
			if ((maze[coord.x][coord.y] & dir) == 1){
				//Check the value of the accessible neighbor
				Tuple<Integer,Integer> neighCoord = bearingCoord(coord, dir);
				if ((double) distMaze[neighCoord.x][neighCoord.y] < minVal){minVal = (double) distMaze[neighCoord.x][neighCoord.y];}
			}
		}
		//Check that the value of the coord argument is one greater than the minimum value
		if (((int) minVal+1)==distMaze[coord.x][coord.y]){
			returnBool = true;
		}
		
		return returnBool;
	}
	
	public void floodFillUpdate(Tuple<Integer,Integer> currCoord){
		Stack<Tuple<Integer,Integer>> entries = new Stack<Tuple<Integer,Integer>>();
		entries.push(currCoord);
		while(!entries.empty()){
			//Pop an entry from the stack
			Tuple<Integer,Integer> workingEntry = entries.pop();
			
		}
	}
	
	public void floodFill(){
		Tuple<Integer,Integer> currCoord = new Tuple<Integer,Integer>(0,0);
		/*Integer representation of heading
		 * 1 = N
		 * 4 = E
		 * 2 = S
		 * 8 = W
		 */
		int heading = 1;
		boolean accessible = false;

	}
	
	private void generateMaze(int cx, int cy) {
		DIR[] dirs = DIR.values();
		Collections.shuffle(Arrays.asList(dirs));
		for (DIR dir : dirs) {
			int nx = cx + dir.dx;
			int ny = cy + dir.dy;
			if (between(nx, x) && between(ny, y)
					&& (maze[nx][ny] == 0)) {
				maze[cx][cy] |= dir.bit;
				maze[nx][ny] |= dir.opposite.bit;
				distMaze[cx][cy] = 0;
				distMaze[nx][ny] = 0;
				generateMaze(nx, ny);
			}
		}
	}
 
	private static boolean between(int v, int upper) {
		return (v >= 0) && (v < upper);
	}
 
	private enum DIR {
		N(1, 0, -1), S(2, 0, 1), E(4, 1, 0), W(8, -1, 0);
		private final int bit;
		private final int dx;
		private final int dy;
		private DIR opposite;
 
		// use the static initializer to resolve forward references
		static {
			N.opposite = S;
			S.opposite = N;
			E.opposite = W;
			W.opposite = E;
		}
 
		private DIR(int bit, int dx, int dy) {
			this.bit = bit;
			this.dx = dx;
			this.dy = dy;
		}
	};
 
	public static void main(String[] args) {
		int x = args.length >= 1 ? (Integer.parseInt(args[0])) : 8;
		int y = args.length == 2 ? (Integer.parseInt(args[1])) : 8;
		MazeGenerator maze = new MazeGenerator(x, y);
		maze.display();
		maze.mark();
		maze.dirmark();
	}
 
}