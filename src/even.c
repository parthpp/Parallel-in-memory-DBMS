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
#include "Parallel_Bucket_Sort.h"

//TODO For testing
#include "Query_Processor.h"
#include <string.h>
void process_result(Query *query);

void set_rank_and_world_size();
void process_company_sale();
void process_sale_by_date(Query * query);
void dummy_table_sale_by_date_result(sale_by_date_result **my_result, int *no_of_elements);
void dummy_table_company_sale_result(company_sale_result **my_result, int *no_of_elements);

void merge_total_sale_by_date(sale_by_date_result *query_result, int query_result_size,
		sale_by_date_result ** final_result, int *final_result_size);


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

	INITIAL_MERGE_TOTAL_BUFFER_SIZE = 1000;

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
			user_query.start_year = 2014;
			user_query.start_month = 1;
			user_query.start_day = 21;
			user_query.end_year = 2015;
			user_query.end_month = 2;
			user_query.end_day = 3;
			//TODO
			//Query new_query;
			//get_input(&new_query);
		}

//		printf("Process: %d : Even_rank : %d : Before query id: %d: End date: %d\n",
//						my_rank, my_even_communicator_rank, user_query.query_id, user_query.end_date.day);

		MPI_Bcast(&user_query, 1, query_type, PROCESS_ZERO, EVEN_COMMUNICATOR);
		printf("Process: %d : Even_rank : %d : Received query id: %d:",
				my_rank, my_even_communicator_rank, user_query.query_id);

		MPI_Isend(&user_query, 1, query_type, my_odd_partner_rank, query_tag, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, MPI_STATUS_IGNORE);
		printf("Process: %d : sended query_id:%d: to its odd partner : %d\n", my_rank, user_query.query_id,
				my_odd_partner_rank);
		++query_tag;
		process_result(&user_query);
		//TODO refine to work when user enters zero
		break;

	}
}

void process_result(Query *query) {
	if (query -> query_id == 1) {
		process_company_sale();
	}
		else if (query -> query_id == 2) {
		process_sale_by_date(query);
	}
		//else if (query -> query_id == 3) {
//		process_delete_record();
//	} else {
//		process_exit();
//	}
}
//

void process_sale_by_date(Query * query) {
	sale_by_date_result *sbd_result;
	unsigned long sale_by_date_result_size;
	sale_by_date_result * final_result;
	int final_result_size;

 //Assuming for the moment that the result is obtained here some how
	sale_by_date_result * my_result;
	int no_of_elements;
	dummy_table_sale_by_date_result(&my_result, &no_of_elements);
	parallel_bucket_sort_sale_by_date(*query, my_result, no_of_elements, &sbd_result, &sale_by_date_result_size);
	merge_total_sale_by_date(sbd_result, sale_by_date_result_size, &final_result, &final_result_size);
	//send_company_sale_result();
}
void process_company_sale() {
	company_sale_result *cs_result;
	unsigned long company_sale_result_size;

 //Assuming for the moment that the result is obtained here some how
	company_sale_result * my_result;
	int no_of_elements;
	dummy_table_company_sale_result(&my_result, &no_of_elements);
	parallel_bucket_sort_company_sale(my_result, no_of_elements, &cs_result, &company_sale_result_size);
	//merge_total_company_sale(cs_result, company_sale_result_size, &final_result, );
	//send_company_sale_result();
}

void dummy_table_sale_by_date_result(sale_by_date_result **my_result, int *no_of_elements) {
	sale_by_date_result *temp_ptr;

	if (my_rank == 0) {
		*no_of_elements = 5;
		get_sale_by_date_result_buffer(5, my_result, &temp_ptr);
		(*my_result)[0].year = 2014;
		(*my_result)[0].month = 1;
		(*my_result)[0].day = 21;

		(*my_result)[0].sales_total = 21210.23;

		(*my_result)[1].year = 2014;
		(*my_result)[1].month = 7;
		(*my_result)[1].day = 8;

		(*my_result)[1].sales_total = 21210.23;

		(*my_result)[2].year = 2014;
		(*my_result)[2].month = 9;
		(*my_result)[2].day = 21;

		(*my_result)[2].sales_total = 21210.23;

		(*my_result)[3].year = 2015;
		(*my_result)[3].month = 1;
		(*my_result)[3].day = 2;

		(*my_result)[3].sales_total = 21210.23;

		(*my_result)[4].year = 2015;
		(*my_result)[4].month = 2;
		(*my_result)[4].day = 3;

		(*my_result)[4].sales_total = 21210.23;

	}

	if (my_rank == 2) {
			*no_of_elements = 7;
			get_sale_by_date_result_buffer(7, my_result, &temp_ptr);
			(*my_result)[0].year = 2014;
			(*my_result)[0].month = 3;
			(*my_result)[0].day = 23;

			(*my_result)[0].sales_total = 21211.23;

			(*my_result)[1].year = 2014;
			(*my_result)[1].month = 7;
			(*my_result)[1].day = 8;

			(*my_result)[1].sales_total = 21211.23;

			(*my_result)[2].year = 2014;
			(*my_result)[2].month = 9;
			(*my_result)[2].day = 21;

			(*my_result)[2].sales_total = 21211.23;

			(*my_result)[3].year = 2014;
			(*my_result)[3].month = 7;
			(*my_result)[3].day = 2;

			(*my_result)[3].sales_total = 21211.23;

			(*my_result)[4].year = 2014;
			(*my_result)[4].month = 9;
			(*my_result)[4].day = 3;

			(*my_result)[4].sales_total = 21211.23;

			(*my_result)[5].year = 2014;
			(*my_result)[5].month = 12;
			(*my_result)[5].day = 22;

			(*my_result)[5].sales_total = 21211.23;

			(*my_result)[6].year = 2015;
			(*my_result)[6].month = 1;
			(*my_result)[6].day = 31;

			(*my_result)[6].sales_total = 21211.23;

		}
}

void dummy_table_company_sale_result(company_sale_result **my_result, int *no_of_elements) {
	company_sale_result *temp_ptr;
	if (my_rank == 0) {
		*no_of_elements = 5;
		get_company_sale_result_buffer(5, my_result, &temp_ptr);
		(*my_result)[0].company_id = 1;
		strcpy((*my_result)[0].company_name, "ABCp0");
		(*my_result)[0].sales_total = 50000.34;

		(*my_result)[1].company_id = 3;
		strcpy((*my_result)[1].company_name, "HIJp0");
		(*my_result)[1].sales_total = 62000.34;

		(*my_result)[2].company_id = 1;
		strcpy((*my_result)[2].company_name, "ABCp0");
		(*my_result)[2].sales_total = 50000.34;

		(*my_result)[3].company_id = 2;
		strcpy((*my_result)[3].company_name, "DEFp0");
		(*my_result)[3].sales_total = 52000.34;

		(*my_result)[4].company_id = 4;
		strcpy((*my_result)[4].company_name, "KLMp0");
		(*my_result)[4].sales_total = 72000.34;
	} else if (my_rank == 2) {
		*no_of_elements = 7;
		get_company_sale_result_buffer(7, my_result, &temp_ptr);
		(*my_result)[0].company_id = 1;
		strcpy((*my_result)[0].company_name, "ABCp2");
		(*my_result)[0].sales_total = 50000.34;

		(*my_result)[1].company_id = 4;
		strcpy((*my_result)[1].company_name, "ABCp2");
		(*my_result)[1].sales_total = 62000.34;

		(*my_result)[2].company_id = 2;
		strcpy((*my_result)[2].company_name, "ABCp2");
		(*my_result)[2].sales_total = 50000.34;

		(*my_result)[3].company_id = 3;
		strcpy((*my_result)[3].company_name, "ABCp2");
		(*my_result)[3].sales_total = 52000.34;

		(*my_result)[4].company_id = 4;
		strcpy((*my_result)[4].company_name, "ABCp2");
		(*my_result)[4].sales_total = 72000.34;

		(*my_result)[5].company_id = 2;
		strcpy((*my_result)[5].company_name, "ABCp2");
		(*my_result)[5].sales_total = 72000.34;

		(*my_result)[6].company_id = 5;
		strcpy((*my_result)[6].company_name, "ABCp2");
		(*my_result)[6].sales_total = 72000.34;

	}
//	} else if (my_rank == 2) {
//
//	} else if (my_rank == 3) {
//
//	}
}

void merge_total_sale_by_date(sale_by_date_result *query_result, int query_result_size,
		sale_by_date_result ** final_result, int *final_result_size) {
	int i;
	int result;
	int no_of_elements = INITIAL_MERGE_TOTAL_BUFFER_SIZE;
	int used_final_result_space = 0;
	sale_by_date_result *final_result_current;

	if (query_result_size == 0) {
		return;
	}

	get_sale_by_date_result_buffer(INITIAL_MERGE_TOTAL_BUFFER_SIZE, final_result, &final_result_current);


	final_result_current = *final_result;

	// Used_final_result_space points to the last element in the buffer
	final_result_current[used_final_result_space] = query_result[0];

	for (i = 1; i != query_result_size; ++i) {
		result = compare_sale_by_date_result(&(final_result_current[used_final_result_space]), &(query_result[i]));
		if (result == 0) {
			final_result_current[used_final_result_space].sales_total += query_result[i].sales_total;
		} else {
			if ((used_final_result_space + 1) == no_of_elements) {
				expand_sale_by_date_result_buffer(&no_of_elements, final_result, &final_result_current);
			}
			++ used_final_result_space;
			final_result_current[used_final_result_space] = query_result[i];
		}
	}

	++used_final_result_space;		// Increment after the last element entered in the loop
	collapse_sale_by_date_result_buffer(used_final_result_space, final_result);

	*final_result_size = used_final_result_space;
	if (my_rank == 0) {

		print_sale(*final_result, used_final_result_space);
	}

}
