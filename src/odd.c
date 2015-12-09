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

void set_odd_process_structures(int read_record_per_count) {
	int read_per_round = read_record_per_count;
	my_even_partner_rank = my_rank - 1;
	read_pending = 0;
	if (signal(SIGALRM, signal_handler) == SIG_ERR) {
		fprintf(stderr, "A signal handler could not be binded for SIGALRM at Process: %d\nThe program shall now terminate\n", my_rank);
		//TODO Terminate program
	}

	get_MPI_Type_for_query(&query_type);

	set_up_data_driver(read_per_round);
}

void start_odd_process(int read_record_per_count) {

	//TODO This is temporary


	Query user_query;
	MPI_Request request;
	int flag = 0;
	MPI_Status status;
	if (my_even_communicator_rank != MPI_UNDEFINED) {
		return;
	}

	set_odd_process_structures(read_record_per_count);

	//TODO Enter User Argument here
	//set_up_data_driver(100);

	alarm(1);
	MPI_Irecv(&user_query, 1, query_type, my_even_partner_rank, 0, MPI_COMM_WORLD, &request);

	while(1) {
		//TODO Data processing

		if (!flag) {
			MPI_Test(&request, &flag, &status);
		} else {
			process_query(&user_query);
			if (user_query.query_id == 4){
				return;
			}
			MPI_Irecv(&user_query, 1, query_type, my_even_partner_rank, 0, MPI_COMM_WORLD, &request);
			flag = 0;
		}
		if (read_pending) {
			insert_data();
			read_pending = 0;
		}
	}
}

void signal_handler(int read_signal) {
	read_pending = 1;

	//TODO Important Remove the commment from alarm
	alarm(2);
}
