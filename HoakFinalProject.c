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
int **readPGM(const char *file_name, int **data);

//	Global Variables
char *originalFile;	//name of maze picture file

int p, my_rank;		//total number of processors, personal processor number
int row, col;		//lengths of maze: width and height
int c;				//integers to track loops, command line

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
 
 int start_x, start_y, end_x, end_y;
 
 printf("Start at (%d, %d) and end at (%d, %d)\n", start_x, start_y, end_x, end_y);
 
 //* End Command line arguements *//

	
	// Allocate maze in chars
	//unsigned char *global_data2_a = (unsigned char *)calloc(row*col, sizeof(unsigned char));
	//unsigned char **global_data2_A = (unsigned char **)malloc(sizeof(unsigned char *)*row);
	
	
	//Outline Code
	
	//Command line takes in dimensions of the maze (l and w)
	
	//Calculate the area each process needs to have, and create the array
	//All processors create their own mini-mazes
	//Process 0 creates the final array and puts the mini-mazes together
	//Process 0 prints the maze to a PGM
	
	//Processors then find at least one way through the maze
	
	
	
	
	
	
	MPI_Finalize();
	
	// Free Memory
	//free(global_data2_A);
	//free(global_data2_a);
}
