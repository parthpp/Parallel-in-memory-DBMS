/*
 * db.c
 *
 *  Created on: Dec 3, 2015
 *      Author: parth
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "BufferCreator.h"
#include "even.h"
#include "odd.h"
#include "Query.h"

void print_communicator_setup_confirmation();
int get_rand_value(int begin, int end);
//MPI Variables
int world_size;
int my_rank;
const int PROCESS_ZERO = 0;

extern MPI_Comm EVEN_COMMUNICATOR;
int my_even_communicator_rank;
int even_communicator_world_size;

int main(int argc, char *argv[]) {

	int read_max;
	int read_record_per_count;


	// Initiliaze MPI Environment.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(argc < 2) {
			printf("Please provide the read max as an option\n");
			return 0;
		} else {
			sscanf(argv[1], "%d", &read_max);
			//read_max = strtol(str_max, NULL,10);
		}

	srand(my_rank);
	read_record_per_count = get_rand_value(10, read_max);

	set_even_process_communicator();

//	print_communicator_setup_confirmation();


	start_even_process();
	start_odd_process(read_record_per_count);
	MPI_Finalize();
	return 0;
}

void print_communicator_setup_confirmation() {
	if (my_even_communicator_rank == MPI_UNDEFINED) {
		printf("Process: %d: World Size: %d\n", my_rank, world_size);
	} else {
		printf("Process: %d : World Size: %d : Even_Process: %d : Even_World_Size: %d\n", my_rank, world_size,
				my_even_communicator_rank, even_communicator_world_size);
	}

}

int get_rand_value(int begin, int end){

	int range = 1 + end - begin;
	int limit = RAND_MAX - (RAND_MAX % range);
	int rand_value;
	do {
		rand_value = rand();
	} while(rand_value >= limit);
	return (rand_value % range) + begin;
}
