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
unsigned char **readPGM(const char *file_name, unsigned char **total_maze);
void writePGM( char *filename, unsigned char **total_maze);
FILE *pgmFile; 	//initial world file

//	Global Variables
char *originalFile;	//name of maze picture file

int p, my_rank;		//total number of processors, personal processor number
int c;				//integers to track loops, command line

int maze_width, maze_length; //dimensions of maze
int start_x, start_y;		 //starting coordinates
int end_x, end_y; 			 //ending coordinates


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
	maze_width = 10;
	maze_length = 10;
 
	//set start and end
	start_x = 1;
	start_y = 0;
	end_x	= 8;
	end_y	= 9;
 
	//print out start and end
	if(my_rank == 0){
		printf("Start at (%d, %d) and end at (%d, %d)\n", start_x, start_y, end_x, end_y);
	}
 	
	// Allocate maze in chars
	unsigned char *total_maze_s1 = (unsigned char *)calloc(maze_width*maze_length, sizeof(unsigned char));
	unsigned char **total_maze = (unsigned char **)malloc(sizeof(unsigned char *)*maze_width);

	
	//fill local with zeros
	for(int k = 0; k<maze_length; k++){
		total_maze[k] = &(total_maze_s1[k*maze_length]); //local_cols from col***
	}
	
	
	//only root process prints full maze
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
	free(total_maze);
	free(total_maze_s1);
}

//write the PGM
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
    fprintf(pgmFile, "%d %d \n", maze_length, maze_width);
    fprintf(pgmFile, "%d \n", 255);	//max grey
    
    //loop to print entire maze
	for ( i = 0; i < maze_width; ++i ) {
		for ( j = 0; j < maze_length; ++j ) {
				
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

// reading the PGM file
/*
unsigned char readPGM(const char *file_name, unsigned char **total_maze)
{
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("File is NULL");
        exit(EXIT_FAILURE);
    }
 
    fgets(version, sizeof(version), pgmFile);
    
    if (strcmp(version, "P2")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }

    fscanf(pgmFile, "%d", &data->col); 
    fscanf(pgmFile, "%d", &data->row);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);
 
    data->matrix = allocate_dynamic_matrix(data->row, data->col);
    

    for (i = 0; i < data->row; ++i){
		for (j = 0; j < data->col; ++j) {
			//lo = fgetc(pgmFile);
				fscanf(pgmFile, "%d", &lo); //scan directly...
                if( lo >= 128) { data->matrix[i][j] = 0; }	//white is dead
                else { data->matrix[i][j] = 1; }			//black means alive
                //printf(" %d", lo);
		}
        // printf("\n");
	}
    fclose(pgmFile);
    return data;
}

*/
