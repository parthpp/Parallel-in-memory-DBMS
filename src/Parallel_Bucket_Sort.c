/*
 * Parallel_Bucket_Sort.c
 *
 *  Created on: Dec 7, 2015
 *      Author: parth
 */

#include "Parallel_Bucket_Sort.h"
#include "BufferCreator.h"
#include "stdlib.h"
#include "Query_Result.h"
#include "Date_Helper.h"

// TODO for testing
#include "Query_Processor.h"
#include <stdio.h>
void print_int_buffer(int * buffer, int length);
void print_date_buffer(Date * buffer, int length);

//int compare_dates(short unsigned int lhs_year, short unsigned int lhs_month, short unsigned int lhs_day,
	//	short unsigned int rhs_year, short unsigned int rhs_month, short unsigned int rhs_day);

void parallel_bucket_sort_company_sale(company_sale_result *my_partner_result, unsigned long my_partner_result_no_of_elements,
		company_sale_result **query_result, unsigned long * query_result_size) {

	company_sale_result *temp_ptr;

	// First Sort the result obtained from the odd partner
	qsort(my_partner_result, my_partner_result_no_of_elements, sizeof(company_sale_result), compare_company_sale_result);

	if(my_rank == 0) {
		printf("Process: %d\n",my_rank);
		printf("Sorted my_partner_result\n");
		print_company_name(my_partner_result, my_partner_result_no_of_elements);
	}
	calculate_company_sale_split_points(my_partner_result, my_partner_result_no_of_elements);

	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Split_Points\n");
		print_int_buffer(split_points, even_communicator_world_size);
	}
	calculate_cs_split_points_index(my_partner_result, my_partner_result_no_of_elements);


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Split_Points_index\n");
		print_int_buffer(split_points_index, even_communicator_world_size);
	}
	calculate_cs_work_division();


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("work_division\n");
		print_int_buffer(work_division, even_communicator_world_size);
	}

	get_int_buffer(even_communicator_world_size, &my_work_size);

	MPI_Alltoall(work_division, 1, MPI_INT, my_work_size, 1, MPI_INT, EVEN_COMMUNICATOR);


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("my_work_size\n");
		print_int_buffer(my_work_size, even_communicator_world_size);
	}
	get_cs_query_result_size(query_result_size);

	printf("Query Result size : %d\n", *query_result_size);
	get_company_sale_result_buffer(*query_result_size, query_result, &temp_ptr);

	calculate_cs_send_displacement();

	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("send_displacement\n");
		print_int_buffer(send_displacement, even_communicator_world_size);
	}
	calculate_cs_receive_displacement();


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("receive_displacement\n");
		print_int_buffer(receive_displacement, even_communicator_world_size);
	}

	get_company_sale_result_MPI_Type(&csr_mpi_type);

	MPI_Alltoallv(my_partner_result, work_division, send_displacement, csr_mpi_type, *query_result,
			my_work_size, receive_displacement, csr_mpi_type, EVEN_COMMUNICATOR);

	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Received Result\n");
		print_company_name(*query_result, *query_result_size);
	}
	qsort(*query_result, *query_result_size, sizeof(company_sale_result), compare_company_sale_result);
//
	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Sorted Result\n");
		print_company_name(*query_result, *query_result_size);
	}
}

void print_int_buffer(int * buffer, int length) {
	int i;
	printf("Process: %d\n", my_rank);
	for (i = 0; i != length; ++i) {
		printf("%d\n", buffer[i]);
	}
}

void print_date_buffer(Date * buffer, int length) {
	int i;
	printf("Process: %d\n", my_rank);
	for (i = 0; i != length; ++i) {
		printf("%d/%d/%d\n", buffer[i].year, buffer[i].month, buffer[i].day);
	}
}
int compare_company_sale_result(const void * lhs, const void *rhs) {
	unsigned long lhs_id = ((company_sale_result *)lhs) -> company_id;
	unsigned long rhs_id = ((company_sale_result *)rhs) -> company_id;

	if (lhs_id > rhs_id) {
		return 1;
	} else if (lhs_id < rhs_id) {
		return -1;
	} else {
		return 0;
	}
}

void calculate_company_sale_split_points(company_sale_result *my_partner_result, unsigned long my_partner_result_no_of_elements) {
	unsigned long highest_company_id;
	unsigned long split_interval;
	int i;
	//Get Buffer to hold split_points
	get_int_buffer(even_communicator_world_size, &split_points);

	// Assuming that according to the specification of the project we have as company_id an auto increment integer begining at 1
	highest_company_id = my_partner_result[my_partner_result_no_of_elements - 1].company_id;

	split_interval = highest_company_id / even_communicator_world_size;

	for (i = 0; i != even_communicator_world_size; ++i) {
		split_points[i] = (i + 1) * split_interval;
	}

	split_points[i -1] = split_points[i - 1] + (highest_company_id % even_communicator_world_size);
}

void calculate_cs_split_points_index (company_sale_result * my_partner_result, unsigned long my_partner_result_no_of_elements) {
	unsigned long first_larger;
	int i;
	get_int_buffer(even_communicator_world_size, &split_points_index);

	for (i = 0; i != even_communicator_world_size; ++i) {
		get_cs_first_larger(my_partner_result, my_partner_result_no_of_elements, split_points[i], &first_larger);
		split_points_index[i] = first_larger;
	}
}

void get_cs_first_larger(company_sale_result * my_partner_result, unsigned long my_partner_result_no_of_elements,
		const unsigned long company_id, unsigned long * first_larger) {

	unsigned long low = 0;
	unsigned long high = my_partner_result_no_of_elements;
	while (low != high) {
		unsigned long mid = (low + high) / 2;
		if (my_partner_result[mid].company_id <= company_id) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}
	*first_larger = low;
}

void calculate_cs_work_division() {
	int i;
	get_int_buffer(even_communicator_world_size, &work_division);

	work_division[0] = split_points_index[0];
	for (i = 1; i != even_communicator_world_size; ++i) {
		work_division[i] = split_points_index[i] - split_points_index[i - 1];
	}
}

void get_cs_query_result_size(unsigned long * query_result_size) {
	int i;
	*query_result_size = 0;
	for (i = 0; i != even_communicator_world_size; ++i) {
		*query_result_size += my_work_size[i];
	}
}

void calculate_cs_send_displacement() {
	int i;
	get_int_buffer(even_communicator_world_size, &send_displacement);

	send_displacement[0] = 0;
	for (i = 1; i != even_communicator_world_size; ++i) {
		send_displacement[i] = split_points_index[i-1];
	}
}

void calculate_cs_receive_displacement() {
	int i;
	get_int_buffer(even_communicator_world_size, &receive_displacement);

	receive_displacement[0] = 0;
	for (i = 1; i != even_communicator_world_size; ++i) {
		receive_displacement[i] = receive_displacement[i-1] + my_work_size[i-1];
	}
}

void parallel_bucket_sort_sale_by_date(Query query, sale_by_date_result *my_partner_result, unsigned long my_partner_result_no_of_elements,
		sale_by_date_result **query_result, unsigned long * query_result_size) {

	sale_by_date_result *temp_ptr;

	// First Sort the result obtained from the odd partner
	qsort(my_partner_result, my_partner_result_no_of_elements, sizeof(sale_by_date_result), compare_sale_by_date_result);

	if(my_rank == 0) {
		printf("Process: %d\n",my_rank);
		printf("Sorted my_partner_result\n");
		print_sale(my_partner_result, my_partner_result_no_of_elements);
	}

	calculate_sale_by_date_split_points(&query, my_partner_result, my_partner_result_no_of_elements);

	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Split_Points\n");
		print_date_buffer(split_points_sbd, even_communicator_world_size);
	}
	calculate_sbd_split_points_index(my_partner_result, my_partner_result_no_of_elements);


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Split_Points_index\n");
		print_int_buffer(split_points_index, even_communicator_world_size);
	}
	calculate_cs_work_division();


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("work_division\n");
		print_int_buffer(work_division, even_communicator_world_size);
	}

	get_int_buffer(even_communicator_world_size, &my_work_size);

	MPI_Alltoall(work_division, 1, MPI_INT, my_work_size, 1, MPI_INT, EVEN_COMMUNICATOR);


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("my_work_size\n");
		print_int_buffer(my_work_size, even_communicator_world_size);
	}
	get_cs_query_result_size(query_result_size);

	printf("Query Result size : %d\n", *query_result_size);
	get_sale_by_date_result_buffer(*query_result_size, query_result, &temp_ptr);

	calculate_cs_send_displacement();

	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("send_displacement\n");
		print_int_buffer(send_displacement, even_communicator_world_size);
	}
	calculate_cs_receive_displacement();


	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("receive_displacement\n");
		print_int_buffer(receive_displacement, even_communicator_world_size);
	}

	get_sale_by_date_result_MPI_Type(&sbd_mpi_type);

	MPI_Alltoallv(my_partner_result, work_division, send_displacement, sbd_mpi_type, *query_result,
			my_work_size, receive_displacement, sbd_mpi_type, EVEN_COMMUNICATOR);

	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Received Result\n");
		print_sale(*query_result, *query_result_size);
	}
	qsort(*query_result, *query_result_size, sizeof(sale_by_date_result), compare_sale_by_date_result);
//
	if (my_rank == 0) {
		printf("Process: %d\n", my_rank);
		printf("Sorted Result\n");
		print_sale(*query_result, *query_result_size);
	}
}

void calculate_sbd_split_points_index (sale_by_date_result * my_partner_result, unsigned long my_partner_result_no_of_elements) {
	unsigned long first_larger;
	int i;
	get_int_buffer(even_communicator_world_size, &split_points_index);

	for (i = 0; i != even_communicator_world_size; ++i) {
		get_sbd_first_larger(my_partner_result, my_partner_result_no_of_elements, &split_points_sbd[i], &first_larger);
		split_points_index[i] = first_larger;
	}
}

void get_sbd_first_larger(sale_by_date_result * my_partner_result, unsigned long my_partner_result_no_of_elements,
		const Date *date, unsigned long * first_larger) {
	unsigned long low = 0;
	int result;
	unsigned long high = my_partner_result_no_of_elements;
	while (low != high) {
		unsigned long mid = (low + high) / 2;
		result = compare_dates(my_partner_result[mid].year, my_partner_result[mid].month, my_partner_result[mid].day,
				date -> year, date -> month, date -> day);
		if (result == -1 || result == 0) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}
	*first_larger = low;
}

void calculate_sale_by_date_split_points(Query *query, sale_by_date_result *my_partner_result, unsigned long my_partner_result_no_of_elements) {

	int year, month, day;
	int i;
	int day_diff = days_between_dates(query -> start_year, query -> start_month, query -> start_day,
				query -> end_year, query -> end_month, query -> end_day);
	int split_interval = day_diff/even_communicator_world_size;

	get_date_buffer(even_communicator_world_size, &split_points_sbd);

	year = query -> start_year;
	month = query -> start_month;
	day = query -> start_day;

	for (i = 0; i != even_communicator_world_size - 1; ++i) {
		add_days_to_date(&year, &month, &day, split_interval);
		split_points_sbd[i].year = year;
		split_points_sbd[i].month = month;
		split_points_sbd[i].day = day;
	}
	// Calculating split points for last process seperately
	split_interval += (day_diff % even_communicator_world_size)-1;
	add_days_to_date(&year, &month, &day, split_interval);
	split_points_sbd[i].year = year;
	split_points_sbd[i].month = month;
	split_points_sbd[i].day = day;
}

void get_date_buffer(int size, Date **buffer) {
	*buffer = (Date *) malloc(size * sizeof(Date)) ;

	if (*buffer == NULL) {
		fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
		//TODO
		//terminate_system();
	}
}

int compare_sale_by_date_result(const void * lhs, const void *rhs) {
	int lhs_year = ((sale_by_date_result *)(lhs)) -> year;
	int lhs_month = ((sale_by_date_result *)(lhs)) -> month;
	int lhs_day = ((sale_by_date_result *)(lhs)) -> day;

	int rhs_year = ((sale_by_date_result *)(rhs)) -> year;
	int rhs_month = ((sale_by_date_result *)(rhs)) -> month;
	int rhs_day = ((sale_by_date_result *)(rhs)) -> day;

	if (rhs_year > lhs_year) {
		return -1;
	} else if (rhs_year < lhs_year) {
		return 1;
	} else {
		if (rhs_month > lhs_month) {
			return -1;
		} else if (rhs_month < lhs_month) {
			return 1;
		} else {
			if (rhs_day > lhs_day) {
				return -1;
			} else if (rhs_day < lhs_day) {
				return 1;
			} else {
				return 0;
			}
		}
	}
}

void send_company_sale_result_to_pzero(company_sale_result *final_result, int final_result_size,
		company_sale_result **print_data, int *print_data_size){
	company_sale_result *temp_ptr = final_result;
	int *print_data_receive_size;
	int *receive_displacement;
	int i;
	// Buffer relevant only at process zero
	get_int_buffer(even_communicator_world_size, &print_data_receive_size);

	// Send the size of your data to process zero
	MPI_Gather(&final_result_size,1, MPI_INT, print_data_receive_size, 1, MPI_INT, 0, EVEN_COMMUNICATOR);

	// Calculate total data to be received at process zero
	*print_data_size = 0;
	for (i = 0; i != even_communicator_world_size; ++i) {
		*print_data_size += print_data_receive_size[i];
	}

	// Allocate buffer
	get_company_sale_result_buffer(*print_data_size, print_data, &temp_ptr);

	get_int_buffer(even_communicator_world_size, &receive_displacement);

	receive_displacement[0] = 0;
	for (i = 1; i != even_communicator_world_size; ++i) {
		receive_displacement[i] = receive_displacement[i-1] + print_data_receive_size[i-1];
	}

	MPI_Gatherv(final_result, final_result_size, csr_mpi_type, *print_data, print_data_receive_size,
			receive_displacement, csr_mpi_type, 0, EVEN_COMMUNICATOR);
}

void send_sale_by_date_result_to_pzero(sale_by_date_result *final_result, int final_result_size,
		sale_by_date_result **print_data, int *print_data_size) {
	sale_by_date_result *temp_ptr = final_result;
	int *print_data_receive_size;
	int *receive_displacement;
	int i;
	// Buffer relevant only at process zero
	get_int_buffer(even_communicator_world_size, &print_data_receive_size);

	// Send the size of your data to process zero
	MPI_Gather(&final_result_size,1, MPI_INT, print_data_receive_size, 1, MPI_INT, 0, EVEN_COMMUNICATOR);

	// Calculate total data to be received at process zero
	*print_data_size = 0;
	for (i = 0; i != even_communicator_world_size; ++i) {
		*print_data_size += print_data_receive_size[i];
	}

	// Allocate buffer
	get_sale_by_date_result_buffer(*print_data_size, print_data, &temp_ptr);

	get_int_buffer(even_communicator_world_size, &receive_displacement);

	receive_displacement[0] = 0;
	for (i = 1; i != even_communicator_world_size; ++i) {
		receive_displacement[i] = receive_displacement[i-1] + print_data_receive_size[i-1];
	}

	MPI_Gatherv(final_result, final_result_size, sbd_mpi_type, *print_data, print_data_receive_size,
			receive_displacement, sbd_mpi_type, 0, EVEN_COMMUNICATOR);
}

