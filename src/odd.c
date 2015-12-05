/*
 * odd.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */


#include "odd.h"
#include "Query.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void start_odd_process() {

	//TODO This is temporary
	my_even_partner_rank = my_rank - 1;
	read_pending = 0;
	signal(SIGALRM, signal_handler);

	Query user_query;
	MPI_Request request;
	int query_tag = 0;
	int flag = 0;
	int j = 0;
	MPI_Status status;
	if (my_even_communicator_rank != MPI_UNDEFINED) {
		return;
	}


	MPI_Irecv(&user_query, 1, query_type, my_even_partner_rank, query_tag, MPI_COMM_WORLD, &request);
	alarm(5);
	while(j != 5) {
		//TODO Data processing

		if (!flag) {
			MPI_Test(&request, &flag, &status);
		}
		if (read_pending) {
			printf("Process : %d: Read Pending: %d", my_rank, j);
			read_pending = 0;
			++j;
		}
	}

	printf("Process: %d: Received : query id: %d :end month: %d: from its partner: %d\n",
			my_rank, user_query.query_id, user_query.end_date.month, my_even_partner_rank);
	++query_tag;
}

void signal_handler(int read_signal) {
	read_pending = 1;
	alarm(5);
}
