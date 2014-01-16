
 
import java.io.IOException;
import java.util.Collections;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Stack;

 
/*
 * recursive backtracking algorithm
 * shamelessly borrowed from the ruby at
 * http://weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking
 */ 

public class MazeGenerator {
	int x;
	int y;
	private static int xMid;
	private static int yMid;
	static int[][] maze;
	private static int[][] distMaze;
	private static HashMap<Tuple<Integer,Integer>,Integer> visited;
	static double inf = Double.POSITIVE_INFINITY;
	static Render render;
 
	public MazeGenerator(int x, int y) {
		this.x = x;
		this.y = y;
		this.xMid = (int) Math.floor((double) this.x/2);
		this.yMid = (int) Math.floor((double) this.y/2);
		maze = new int[this.x][this.y];
		this.render = new Render(x, y, maze);
		distMaze = new int[this.x][this.y];
		zeroDistMaze();
		generateMaze(0, 0);
	}
 
	public MazeGenerator(String filePath) throws IOException {
		Parser parsedMaze = new Parser(filePath);
		this.x = parsedMaze.stringMaze.length;
		this.y = parsedMaze.stringMaze[0].length;
		System.out.println("asdfs");
		System.out.println(this.x);
		xMid = (int) Math.floor((double) this.x/2);
		yMid = (int) Math.floor((double) this.y/2);
		maze = parsedMaze.stringMaze;
		render = new Render(x, y, maze);
		distMaze = new int[this.x][this.y];
		zeroDistMaze();
	}
	
	//Fill the distMaze with zeros
	public void zeroDistMaze(){
		for(int[] column : distMaze){
			Arrays.fill(column, 0);
		}
	}

	public static int calcDist(int x, int y) {
		int dist = Math.abs(yMid - y)+Math.abs(xMid - x);
		return dist;
	}
	
	public static String distString(int dist){
		String distVal;
		if (dist > 9){
			distVal = Integer.toString(dist);
		}else{
			distVal = "0"+Integer.toString(dist);
		}
		return distVal;
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
		render.display();
	}
 
}