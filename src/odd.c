/*
 * odd.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */


#include "odd.h"
#include <stdio.h>

void start_odd_process() {

	//TODO This is temporary
	my_even_partner_rank = my_rank - 1;

	int query_id;
	MPI_Request request;
	int query_tag = 0;
	int flag = 0;
	MPI_Status status;
	if (my_even_communicator_rank != MPI_UNDEFINED) {
		return;
	}


	MPI_Irecv(&query_id, 1, MPI_INT, my_even_partner_rank, query_tag, MPI_COMM_WORLD, &request);
	while(1) {
		//TODO Data processing
		MPI_Test(&request, &flag, &status);
		if (flag) {
			break;
		}
	}

	printf("Process: %d: Received: %d: from its partner: %d\n", my_rank, query_id, my_even_partner_rank);
	++query_tag;

}
