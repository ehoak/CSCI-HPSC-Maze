///////////////////////////////
//   Elanor Hoak
//   Final Project
//	 Parallelize a Maze
///////////////////////////////

//	Include packages
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "mpi.h"	//verson 2.0.0

//	Functions

//Read and Write PGM
unsigned char **readPGM(const char *file_name, unsigned char **total_maze);
void writePGM( char *filename, unsigned char **total_maze);
FILE *pgmFile; 	//initial world file

//print matrix to command line (for small maze debugging)
void printMatrix(unsigned char **total_maze);

//Memory allocation and deallocation
unsigned char **allocate_maze(int row, int col);
void deallocate_dynamic_matrix(unsigned char **matrix, int row);

int **allocate_stack(int num);
void deallocate_stack(int **matrix, int row);

//	Global Variables
char *originalFile;	//name of maze picture file

int p, my_rank;		//total number of processors, personal processor number

int maze_rows, maze_cols; 	//dimensions of maze
int start_c, start_r;		//starting coordinates
int end_c, end_r; 			//ending coordinates

int local_cols, local_rows;	//local length and width of processor area

/////////////////////////////
//	Project parallelizes a maze generator
//	and finding the path through the maze.
/////////////////////////////

main(int argc, char* argv[]) {
	
	//	MPI variables
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Status status;
	
	//*	Command line arguements *//
	/*
	while ((c = getopt(argc, argv, "f:")) != EOF){
      switch(c) {
      case 'f':					//file name
		originalFile = optarg;
        break;
      }
	}
 */

 
	//start and end are set to somewhere along the edges,
	// the only blank spaces in the outer frame
 
	//set maze dimentions
	// (Note: there is a max size in order to view the PGM,
	// 		but program runs for all sizes)
	maze_cols = 10;
	maze_rows = 10;
	
	//**** Something is erronious with non-square worlds
 
	//set start and end
	start_c = 1;
	start_r = 1;
	end_c	= 8;
	end_r	= 9;
 
	//print out start and end
	if(my_rank == 0){
		printf("Start at (%d, %d) and end at (%d, %d)\n", start_c, start_r, end_c, end_r);
	}
 	
	// Allocate maze in unsigned chars
	unsigned char **total_maze = allocate_maze(maze_rows, maze_cols);
	
	//allocate "explored" stack - ****
	//Change to be places where a desicion occurred,
	// so can retest that when backtracking
	int **explored = allocate_stack(maze_cols*maze_rows);
	int exp = 0; //number of explored squares
	
	//allocate "walls" stack
	int **foundwalls = allocate_stack(maze_cols*maze_rows);
	int walls = -1; //index number for explored walls
	
	
	/////////////////////
	//make maze in array of walls - 0 = wall
	/////////////////////

	
	//initial condition
	
	int row, col; 	//track current row and col values
	row = start_r;
	col = start_c;
	
	int n, s, e, w; //bool values for if this direction can be visited
	int avail = 1;	//boolean for available 
	int gone;		//track previous explored so keep walls
	int r; 			//random variable
	int tr = 1; 	//track iterations
	
	srand(time(NULL)); //make random seed out of time for future random variables
	
	
	//do the following while there are still places to explore
	while(avail > 0){
	//for(int tr = 0; tr < 9; tr++){
		//set current to blank
		total_maze[row][col] += 1;
		
		//add current cell to explored list
		explored[exp][0] = row;
		explored[exp][1] = col;
		exp++;
		
		//set all directions initially to true, as they are most of the time
		n = 1;
		s = 2;
		e = 3;
		w = 4;
		
		//get acceptable neighbors
	
		//check north - false if against top edge or north is on path
		if(row <= 1 || total_maze[row-1][col] > 0){
			n = 0;
		}
		else{
			//check walls to see if bordering known wall
			for( gone = 0; gone < walls; gone++){
				if(foundwalls[gone][0] == row-1 && (foundwalls[gone][1] == col-1 || foundwalls[gone][1] == col+1)){
					n = 0;	
				}
			}
			
			//see if condition still not false, then add wall if needed
			if(n != 0){
				//check diagonals to make sure is really valed, to keep walls intact
				//for( gone = 0; gone < exp; gone++){
					if(total_maze[row-1][col-1] == 1 || total_maze[row-1][col+1] == 1){
					
					//if(explored[gone][0] == row-1 && (explored[gone][1] == col-1 || explored[gone][1] == col+1)){
						//add new wall to wall list
						walls++;
						foundwalls[walls][0] = row-1;
						foundwalls[walls][1] = col;
						
						total_maze[row-1][col] += 5; //*** add to show wall
						
						n = 0;
					}
				//}
			}
		}// end north
		
		
		//check south
		if(row >= maze_rows-2 || total_maze[row+1][col] > 0 ){
			s = 0;
		}
		else{ //check if bordering wall
			for( gone = 0; gone < walls; gone++){
				if(foundwalls[gone][0] == row+1 && (foundwalls[gone][1] == col-1 || foundwalls[gone][1] == col+1)){
					s = 0;
				}
			}
			
			//add wall if corner
			if(s != 0){
				//check diagonals
				if(total_maze[row+1][col-1] == 1 || total_maze[row+1][col+1] == 1){
					//add new wall to wall list
					walls++;
					foundwalls[walls][0] = row+1;
					foundwalls[walls][1] = col;
					
					total_maze[row+1][col] += 5; //*** add to show wall
					
					s = 0;
				}
			}
		}
		
		//check east
		if(col >= maze_cols-2 || total_maze[row][col+1] > 0 ){
			e = 0;
		}
		else{ //check if bordering wall
			for( gone = 0; gone < walls; gone++){
				if(foundwalls[gone][0] == col+1 && (foundwalls[gone][1] == row-1 || foundwalls[gone][1] == row+1)){
					e = 0;
				}
			}
			
			//add wall if corner
			if(e != 0){
				//check diagonals
				if(total_maze[row+1][col+1] == 1 || total_maze[row-1][col+1] == 1){
					//add new wall to wall list
					walls++;
					foundwalls[walls][0] = row;
					foundwalls[walls][1] = col+1;
					
					total_maze[row][col+1] += 3; //*** add to show wall
					
					e = 0;
				}
			}
		} // end east
		
		//****east or west has something on the wrong side
		
		//check west
		if(col <= 1 || total_maze[row][col-1] > 0 ){
			w = 0;
		}
		else{ //check if bordering wall
			for( gone = 0; gone < walls; gone++){
				if(foundwalls[gone][0] == col-1 && (foundwalls[gone][1] == row-1 || foundwalls[gone][1] == row+1)){
					w = 0;
				}
			}
			
			//add wall if corner
			if(w != 0){
				//check diagonals
				if(total_maze[row+1][col-1] == 1 || total_maze[row-1][col-1] == 1){
					//add new wall to wall list
					walls++;
					foundwalls[walls][0] = row;
					foundwalls[walls][1] = col-1;
					
					total_maze[row][col-1] += 4; //*** add to show wall
					
					w = 0;
				}
			}
		}//end west
		
		//set to true to roll at least once
		int roll = 1;
		
		avail = n+s+e+w;
		
		//check if no path
		if(avail == 0){
			printf("Go back! No forward!\n");
			roll = 0;
		}
	
		//keep generating random int until valid direction chosen

		while(roll){ //repeat until true
		
			r = (rand() % 4) + 1; //do not include 0, so add one
	
			if(r == n) { //go north
				row -= 1;
				roll = 0;
			}
			else if(r == s){ //go south
				row += 1;
				roll = 0;
			}
			else if(r = e){ //go east
				col += 1;
				roll = 0;
			}
			else if(r = w){ //go west
				col -= 1;
				roll = 0;
			}
		}
		printf("%d: Available: n %d s %d e %d w %d, rand = %d\n",tr, n, s, e, w, r);
	
		printf("Went to: %d, %d\n", row, col);
		tr++;
	
	}//end for loop
	
	//make last a blank
	total_maze[row][col] == 1;
	
	
	//print blank maze to command line
	printMatrix(total_maze);
	
	//only root process prints full maze - done
	
	
	  
	if(my_rank == 0){
		char *outputName = "testPicture.PGM";
		writePGM( outputName, total_maze );
	}
	
	
	
	//Outline Code
	
	//Command line takes in dimensions of the maze (l and w)
	
	//Calculate the area each process needs to have, and create the array
	//All processors create their own mini-mazes
	//Process 0 creates the final array and puts the mini-mazes together
	//Process 0 prints the maze to a PGM
	
	//Processors then find at least one way through the maze
	
	
	MPI_Finalize();
	
	// Free Memory
	deallocate_dynamic_matrix(total_maze, maze_rows);
	deallocate_stack(explored, maze_cols*maze_rows);
	deallocate_stack(foundwalls, maze_cols*maze_rows);
}

// write the PGM
void writePGM( char *filename, unsigned char **total_maze)
{
	//declare local variables
	int i, j;		//count loops through width and length
	int place;		//the value to place in PGM
	
	// OPEN PGM FILE
    pgmFile = fopen(filename, "wb");
    
    //check for error
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }
	
	//write header
    fprintf(pgmFile, "P2 \n");		//a P3 filetype
    fprintf(pgmFile, "%d %d \n", maze_rows, maze_cols);
    fprintf(pgmFile, "%d \n", 255);	//max grey
    
    //loop to print entire maze
	for ( i = 0; i < maze_rows; ++i ) {
		for ( j = 0; j < maze_cols; ++j ) {
				
				//get value to insert
				place = total_maze[i][j];
				
				//print black or white if space is filled or empty
                if(place == 1){
					fprintf(pgmFile, "%d ", 255);
				}
                else {
					fprintf(pgmFile, "  %d ", 0);
				}
		}
			//insert endline for next loop
			fprintf(pgmFile, "\n");
	}
    
    // CLOSE PGM FILE
    fclose(pgmFile);

}

void printMatrix(unsigned char **total_maze){
	int i, j;
	
	for (i = 0; i < maze_rows; i++){
		for (j = 0; j < maze_cols; j++){
			printf("%u ", total_maze[i][j]);
		}
		printf("\n");
	}
}

// allocate memory for matrix
unsigned char **allocate_maze(int row, int col)
{
	int i; //for loop
	
	//allocate the whole size based on dimensions
    unsigned char **ret_val = (unsigned char **)malloc(sizeof(unsigned char *)*row);
	
	//fill with allocated cols set to zero
    for (i = 0; i < row; i++) {
        ret_val[i] = (unsigned char *)calloc(col, sizeof(unsigned char));
    }
    
    // return double pointer to (x,y) matrix maze base
    return ret_val;
}

// allocate stack array
int **allocate_stack(int num)
{
	int i; //for loop
	
	//allocate the whole size based on dimensions
    int **ret_val = (int **)malloc(sizeof(int *)*num);
	
	//fill with allocated cols set to zero
    for (i = 0; i < num; i++) {
        ret_val[i] = (int *)calloc(2, sizeof(int));
    }
    
    // return double pointer to (x,y) of stack
    return ret_val;
}

//free maze memory
void deallocate_dynamic_matrix(unsigned char **matrix, int row)
{
	int i; // for loop
	
	//free each col
    for (i = 0; i < row; ++i){
        free(matrix[i]);
	}
	
	//free whole thing
    free(matrix);
}

//free explored stack memory
void deallocate_stack(int **matrix, int row)
{
	int i; // for loop
	
	//free each
    for (i = 0; i < row; ++i){
        free(matrix[i]);
	}
	
	//free whole thing
    free(matrix);
}
