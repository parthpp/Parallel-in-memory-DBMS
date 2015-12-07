/*
 * odd.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */


#include "odd.h"
#include "Query.h"
#include "DBDataDriver.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "Query_Processor.h"

void set_odd_process_structures() {
	int read_per_round = 2;
	my_even_partner_rank = my_rank - 1;
	read_pending = 0;
	if (signal(SIGALRM, signal_handler) == SIG_ERR) {
		fprintf(stderr, "A signal handler could not be binded for SIGALRM at Process: %d\nThe program shall now terminate\n", my_rank);
		//TODO Terminate program
	}

	get_MPI_Type_for_query(&query_type);

	set_up_data_driver(read_per_round);
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
	//alarm(2);
	while(j != 6) {
		//TODO Data processing

		if (!flag) {
			MPI_Test(&request, &flag, &status);
		} else {
			//TODO important remove

			break;
		}
		if (read_pending) {
			insert_data();
			read_pending = 0;
			++j;
		}


	}

	printf("Process: %d: Received : query id: %d : from its partner: %d\n",
			my_rank, user_query.query_id, my_even_partner_rank);
	++query_tag;
}

void signal_handler(int read_signal) {
	read_pending = 1;
	alarm(2);
}
