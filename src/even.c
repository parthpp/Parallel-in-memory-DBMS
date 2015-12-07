/*
 * even.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#include "even.h"
#include "BufferCreator.h"
#include <stdio.h>
#include <stdlib.h>
#include "Query_Result.h"

void set_rank_and_world_size();


void set_even_process_communicator() {
	int *even_process_ranks;
	int i = 0, j = 0;
	MPI_Group my_group;
	MPI_Group my_added_group;


	if(world_size % 2 != 0) {
		fprintf(stderr,"Process: %d Error: The total number of processes should be an even number.\n"
				"The program can not continue in this state.\n"
				"Hence Terminating", my_rank);
		//TODO
		//terminate_system();
	}

	get_int_buffer((world_size/2), &even_process_ranks);

	while (i < world_size - 1) {
		even_process_ranks[j] = i;
		++j;
		i += 2;
	}

	MPI_Comm_group(MPI_COMM_WORLD, &my_group);
	MPI_Group_incl(my_group, (world_size/2), even_process_ranks, &my_added_group);
	MPI_Comm_create(MPI_COMM_WORLD, my_added_group, &EVEN_COMMUNICATOR);

	//TODO
	//Destroy EVEN_Communicator and COrresponding group

	free(even_process_ranks);

	set_rank_and_world_size();

}

void set_rank_and_world_size() {

	// For odd processes the EVEN_COMMUNICATOR will be set to MPI_COMM_NULL by the MPI_Comm_create call
	if (EVEN_COMMUNICATOR == MPI_COMM_NULL) {
		my_even_communicator_rank = MPI_UNDEFINED;
		even_communicator_world_size = MPI_UNDEFINED;
		my_odd_partner_rank = MPI_UNDEFINED;
	} else {
		MPI_Comm_size(EVEN_COMMUNICATOR, &even_communicator_world_size);
		MPI_Comm_rank(EVEN_COMMUNICATOR, &my_even_communicator_rank);
		my_odd_partner_rank = my_rank + 1;
	}
}

void start_even_process() {
	get_MPI_Type_for_query(&query_type);

	Query user_query;
	MPI_Request request;

	int query_tag = 0;
	if (my_even_communicator_rank == MPI_UNDEFINED) {
		return;
	}

	while (1) {
		if (my_rank == PROCESS_ZERO) {
			user_query.query_id = 2;
			user_query.start_year = 2013;
			user_query.start_month = 12;
			user_query.start_day = 18;
			user_query.end_year = 2015;
			user_query.end_month = 12;
			user_query.end_day = 17;
			//TODO
			//Query new_query;
			//get_input(&new_query);
		}

//		printf("Process: %d : Even_rank : %d : Before query id: %d: End date: %d\n",
//						my_rank, my_even_communicator_rank, user_query.query_id, user_query.end_date.day);

		MPI_Bcast(&user_query, 1, query_type, PROCESS_ZERO, EVEN_COMMUNICATOR);
		printf("Process: %d : Even_rank : %d : Received query id: %d: End date: %d\n",
				my_rank, my_even_communicator_rank, user_query.query_id, user_query.end_day);

		user_query.query_id += my_rank;
		MPI_Isend(&user_query, 1, query_type, my_odd_partner_rank, query_tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, MPI_STATUS_IGNORE);
		printf("Process: %d : sended end month:%d: to its odd partner : %d\n", my_rank, user_query.end_month,
				my_odd_partner_rank);
		++query_tag;
		//TODO refine to work when user enters zero
		break;

	}
}

//void process_result(Query *query) {
//	if (query -> query_id == 1) {
//		process_company_sale();
//	} else if (query -> query_id == 2) {
//		process_sale_by_date();
//	} else if (query -> query_id == 3) {
//		process_delete_record();
//	} else {
//		process_exit();
//	}
//}
//
//void process_company_sale() {
//	company_sale_result *cs_result;
//	unsigned long company_sale_result_size;

// Assuming for the moment that the result is obtained here some how
// comapny_sale_result * my_result;
//	parallel_bucket_sort_company_sale(my_result, &cs_result, &company_sale_result_size);
//	merge_total_company_sale(cs_result, company_sale_result_size);
//	send_company_sale_result();
//}
