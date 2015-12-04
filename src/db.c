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

void print_communicator_setup_confirmation();
//MPI Variables
int world_size;
int my_rank;

extern MPI_Comm EVEN_COMMUNICATOR;
int my_even_communicator_rank;
int even_communicator_world_size;

int main(int argc, char *argv[]) {

	// Initiliaze MPI Environment.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	set_even_process_communicator();

	print_communicator_setup_confirmation();

	start_even_process();
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
