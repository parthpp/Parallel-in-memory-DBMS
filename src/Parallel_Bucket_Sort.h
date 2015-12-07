/*
 * Parallel_Bucket_Sort.h
 *
 *  Created on: Dec 7, 2015
 *      Author: parth
 */

#ifndef PARALLEL_BUCKET_SORT_H_
#define PARALLEL_BUCKET_SORT_H_

#include "Query_Result.h"
#include "mpi.h"
#include "Query.h"
#include "Query_Processor.h"

typedef struct {
	int year;
	int month;
	int day;
} Date;

//extern int world_size;
extern MPI_Comm EVEN_COMMUNICATOR;
extern int even_communicator_world_size;
extern int my_rank;
// Buffer of company_id that will store the split_points from where the data is to be divided.
int *split_points;

// Index corresponding to the split points in the data buffer
int *split_points_index;

// Buffer consisting of the number of elements that it will send to each process in EVEN_COMMUNICATOR
int *work_division;

int *my_work_size;

int *send_displacement;

int *receive_displacement;

MPI_Datatype csr_mpi_type;

// Variables for Sale_By_Date Query
Date * split_points_sbd;
MPI_Datatype sbd_mpi_type;

void parallel_bucket_sort_company_sale(company_sale_result *my_partner_result, unsigned long my_partner_result_no_of_elements,
		company_sale_result **query_result, unsigned long * query_result_size);
int compare_company_sale_result(const void * lhs, const void *rhs);
void calculate_company_sale_split_points(company_sale_result *my_partner_result, unsigned long my_partner_result_no_of_elements);
void calculate_cs_split_points_index (company_sale_result * my_partner_result, unsigned long my_partner_result_no_of_elements);
void get_cs_first_larger(company_sale_result * my_partner_result, unsigned long my_partner_result_no_of_elements,
		const unsigned long company_id, unsigned long * first_larger);
void calculate_cs_work_division();
void get_cs_query_result_size(unsigned long * query_result_size);
void calculate_cs_send_displacement();
void calculate_cs_receive_displacement();


void parallel_bucket_sort_sale_by_date(Query query, sale_by_date_result *my_partner_result, unsigned long my_partner_result_no_of_elements,
		sale_by_date_result **query_result, unsigned long * query_result_size);
int compare_sale_by_date_result(const void * lhs, const void *rhs);
void calculate_sale_by_date_split_points(Query *query, sale_by_date_result *my_partner_result, unsigned long my_partner_result_no_of_elements);
void calculate_sbd_split_points_index (sale_by_date_result * my_partner_result, unsigned long my_partner_result_no_of_elements);
void get_sbd_first_larger(sale_by_date_result * my_partner_result, unsigned long my_partner_result_no_of_elements,
		const Date *date, unsigned long * first_larger);
//void calculate_sbd_work_division();
//void get_sbd_query_result_size(unsigned long * query_result_size);
//void calculate_sbd_send_displacement();
//void calculate_sbd_receive_displacement();

void get_date_buffer(int size, Date **buffer);



#endif /* PARALLEL_BUCKET_SORT_H_ */
