///////////////////////////////
//   Elanor Hoak
//   Final Project
//	 Parallelize a Maze
///////////////////////////////

//	Include packages
#include <stdio.h>
#include <string.h>
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
unsigned char **allocate_dynamic_matrix(int row, int col);
void deallocate_dynamic_matrix(unsigned char **matrix, int row);

//	Global Variables
char *originalFile;	//name of maze picture file

int p, my_rank;		//total number of processors, personal processor number
int c;				//integers to track loops, command line

int maze_rows, maze_cols; //dimensions of maze
int start_c, start_r;		 //starting coordinates
int end_c, end_r; 			 //ending coordinates

int local_cols, local_rows;	 //local length and width of processor area

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
	maze_cols = 200;
	maze_rows = 200;
 
	//set start and end
	start_c = 1;
	start_r = 0;
	end_c	= 8;
	end_r	= 9;
 
	//print out start and end
	if(my_rank == 0){
		printf("Start at (%d, %d) and end at (%d, %d)\n", start_c, start_r, end_c, end_r);
	}
 	
	// Allocate maze in unsigned chars
	unsigned char **total_maze = allocate_dynamic_matrix(maze_rows, maze_cols);
	
	//print blank maze to command line
	//printMatrix(total_maze);
	
	//only root process prints full maze - done
	//*
	  
	if(my_rank == 0){
		char *outputName = "testPicture.PGM";
		writePGM( outputName, total_maze );
	}
	 
	//* */
	
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
                if(place == 0){
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
unsigned char **allocate_dynamic_matrix(int row, int col)
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

//free memory
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
