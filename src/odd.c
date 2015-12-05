/*
 * odd.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */


#include "odd.h"
#include "Query.h"
#include <stdio.h>

void start_odd_process() {

	//TODO This is temporary
	my_even_partner_rank = my_rank - 1;
	Query user_query;
	MPI_Request request;
	int query_tag = 0;
	int flag = 0;
	MPI_Status status;
	if (my_even_communicator_rank != MPI_UNDEFINED) {
		return;
	}


	MPI_Irecv(&user_query, 1, query_type, my_even_partner_rank, query_tag, MPI_COMM_WORLD, &request);
	while(1) {
		//TODO Data processing
		MPI_Test(&request, &flag, &status);
		if (flag) {
			break;
		}
	}

	printf("Process: %d: Received : query id: %d :end month: %d: from its partner: %d\n",
			my_rank, user_query.query_id, user_query.end_date.month, my_even_partner_rank);
	++query_tag;
}
