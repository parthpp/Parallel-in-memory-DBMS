/*
 * db.c
 *
 *  Created on: Dec 3, 2015
 *      Author: parth
 */

#include "mpi.h"
#include <stdio.h>

void get_even_process_communicator(MPI_Comm *new_communicator);
//MPI Variables
int world_size;
int my_rank;

int main(int argc, char *argv[]) {
	MPI_Comm EVEN_COMMUNICATOR;

	// Initiliaze MPI Environment.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	get_even_process_communicator(&EVEN_COMMUNICATOR);


	printf("Process: %d: World Size: %d\n", my_rank, world_size);
	MPI_Finalize();
	return 0;
}


void get_even_process_communicator(MPI_Comm *new_communicator) {
	int *even_process_ranks;
	int i;
	MPI_Group my_group;
	MPI_Group my_added_group;


	if(world_size % 2 != 0) {
		fprintf("Process: %d Error: The total number of processes should be an even number.\n"
				"The program can not continue in this state.\n"
				"Hence Terminating", my_rank);
		//TODO
		//terminate_system();
	}

	get_int_buffer(world_size/2, &even_process_ranks);
	for (i = 0; i != (world_size - 2); i + 2) {
		even_process_ranks[i] = i;
	}

	MPI_Comm_group(MPI_COMM_WORLD, &my_group);
	MPI_Group_incl(my_group, (world_size/2), even_process_ranks, &my_added_group);
	MPI_Comm_create(MPI_COMM_WORLD, my_added_group, new_communicator);
}
