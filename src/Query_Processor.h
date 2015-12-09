/*
 * Query_Processor.h
 *
 *  Created on: Dec 6, 2015
 *      Author: parth
 */

#ifndef QUERY_PROCESSOR_H_
#define QUERY_PROCESSOR_H_

#include "Query.h"
#include "Query_Result.h"
#include "Record.h"
#include "Shared_Memory.h"

// Buffer related variables
extern int data_buffer_size;				// Size of Current Buffer
extern Record * data_buffer_begin;			// POinter to the beigining i.e the first element of the buffer
extern Record * data_buffer_current;		// Pointer the next empty location in the buffer(i.e place where next element will go)
extern int used_buffer_size;				// number of elements in the buffer
extern int my_rank;
extern int my_even_partner_rank;

int INITIAL_RESULT_SIZE;

void process_query(Query *query);

//TODO For testing
void print_company_name(company_sale_result * result_begin, int used_temp_buffer_size);
void print_sale(sale_by_date_result * result_begin, int used_temp_buffer_size);

int compare_dates(short unsigned int lhs_year, short unsigned int lhs_month, short unsigned int lhs_day,
		short unsigned int rhs_year, short unsigned int rhs_month, short unsigned int rhs_day);
void terminate_odd();


#endif /* QUERY_PROCESSOR_H_ */
