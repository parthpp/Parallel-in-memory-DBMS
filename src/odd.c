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

void set_odd_process_structures() {
	my_even_partner_rank = my_rank - 1;
	read_pending = 0;
	if (signal(SIGALRM, signal_handler) == SIG_ERR) {
		fprintf(stderr, "A signal handler could not be binded for SIGALRM at Process: %d\nThe program shall now terminate\n", my_rank);
		//TODO Terminate program
	}

	get_MPI_Type_for_query(&query_type);
}

void start_odd_process() {

	//TODO This is temporary


	Query user_query;
	MPI_Request request;
	int query_tag = 0;
	int flag = 0;
	int j = 0;
	MPI_Status status;
	if (my_even_communicator_rank != MPI_UNDEFINED) {
		return;
	}

	set_odd_process_structures();

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
			my_rank, user_query.query_id, user_query.end_month, my_even_partner_rank);
	++query_tag;
}

void signal_handler(int read_signal) {
	read_pending = 1;
	alarm(5);
}
