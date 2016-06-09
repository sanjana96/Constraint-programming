import java.util.*;
class sudoku5{
	static int grid[][] = new int[9][9];
	static boolean domain[][][] = new boolean[9][9][10];
	static boolean doneRow[][] = new boolean[9][10];
	static boolean doneCol[][] = new boolean[9][10];
	static int n = 9;
	static int filled=0;
	public static void main(String args[]){
		Scanner s = new Scanner(System.in);
		for(int i=0;i<n;i++)
			for(int j=0;j<n;j++)
				for(int k=1;k<10;k++)
					domain[i][j][k] = true;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				int temp = s.nextInt();
				grid[i][j] = temp;
				if(temp!=0){
					doneRow[i][temp] = true;
					doneCol[j][temp] = true;
					pruneDomain(i,j);
					filled++;
				}
			}
		}
		while(filled!=81){	
			deduce();
		}
		printGrid();
	}
	public static void pruneDomain(int i,int j){
		int num = grid[i][j];
		for(int x=0;x<n;x++)domain[x][j][num] = false;
		for(int x=0;x<n;x++)domain[i][x][num] = false;
		int startCol = (j/3)*3;
		int startRow = (i/3)*3;
		for(int x=startRow;x<startRow+3;x++){
			for(int y=startCol;y<startCol+3;y++){
					domain[x][y][num] = false;
			}
		}
	}
	public static void deduce(){
		int filledBefore = filled;
		for(int num=1;num<10;num++){
			for(int row=0;row<9;row++){
				if(!doneRow[row][num]){
				int count=0;int c=0;
				for(int col=0;col<9;col++){
					if(grid[row][col]==0 && domain[row][col][num]){
						count++;c = col;
					}
				}
				if(count==1){
					grid[row][c]=num;
					filled++;
					doneRow[row][num] = true;
					doneCol[c][num] = true;
					pruneDomain(row,c);
				}
				}
			}
			for(int col=0;col<9;col++){
				if(!doneCol[col][num]){
				int count=0;int r=0;
				for(int row=0;row<9;row++){
					if(grid[row][col]==0 && domain[row][col][num]){
						count++;r = row;
					}
				}
				if(count==1){
					grid[r][col]=num;
					filled++;
					doneRow[r][num] = true;
					doneCol[col][num] = true;
					pruneDomain(r,col);
				}
				}
			}
			for(int sq=0;sq<9;sq++){
				int count=0;int count1=0;int c=0;int r=0;
				int rstart=(sq%3)*3;int cstart=(sq/3)*3;
				for(int row=rstart;row<rstart+3;row++){
					for(int col=cstart;col<cstart+3;col++){
						if(grid[row][col]==num)count++;
						else if(grid[row][col]==0 && domain[row][col][num]){
							count1++;c = col;r = row;
						}
					}
				}
				if(count==0 && count1==1){
					grid[r][c]=num;
					filled++;
					doneRow[r][num] = true;
					doneCol[c][num] = true;
					pruneDomain(r,c);
				}
			}
		}
		if(filledBefore==filled){
			System.out.println("Not logically solvable!");
				filled = 81;
		}
	}
	public static void printGrid(){
		System.out.println(" - - - - - - - - - - - - -");
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(j%3==0 && j/3==0)
					System.out.print(" | "+grid[i][j]+" ");
				else if(j%3==2)System.out.print(grid[i][j]+" | ");
				else System.out.print(grid[i][j]+" ");
			}
			if(i%3==2){
				System.out.println();
				System.out.println(" - - - - - - - - - - - - -");
			}
			else System.out.println();
		}
	}
}
