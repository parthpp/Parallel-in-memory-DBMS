/*
 * Query_Processor.c
 *
 *  Created on: Dec 6, 2015
 *      Author: parth
 */

#include "Query_Processor.h"
#include <string.h>
#include <stdio.h>

void company_sale();
void sale_by_date(Query *query);
void delete_record(Query *query);
void terminate_system();
void print_company_name(company_sale_result * result_begin, int used_temp_buffer_size);
void print_sale(sale_by_date_result * result_begin, int used_temp_buffer_size);

int compare_dates(short unsigned int lhs_year, short unsigned int lhs_month, short unsigned int lhs_day,
		short unsigned int rhs_year, short unsigned int rhs_month, short unsigned int rhs_day);

void process_query(Query * query) {

	if (query -> query_id == 1) {
		company_sale();
	} else if (query -> query_id == 2) {
		sale_by_date(query);
	} else if (query -> query_id == 3) {
		delete_record(query);
	} else if (query -> query_id == 4) {
		//terminate_system();
	}
}

void sale_by_date(Query *query) {
	// These data variables represents and manages the buffer associated with the result for sale date.
	int result_buffer_size = INITIAL_RESULT_SIZE;
	int used_result_buffer_size = 0;
	sale_by_date_result *result_begin;
	sale_by_date_result *result_current;
	int compare_result;

	int i;
	get_sale_by_date_result_buffer(INITIAL_RESULT_SIZE, &result_begin, &result_current);

	for (i = 0; i != used_buffer_size; ++i) {

		// First expand the buffer if it is empty
		if (used_result_buffer_size == result_buffer_size) {
			expand_sale_by_date_result_buffer(&result_buffer_size, &result_begin, &result_current);
		}

		// Check that the record is not deleted
		if (data_buffer_begin[i].deleted != 1) {

			compare_result = compare_dates(data_buffer_begin[i].year, data_buffer_begin[i].month, data_buffer_begin[i].day,
					query -> start_year, query -> start_month, query -> start_day);
			// Check for start date
			if (compare_result == 1 || compare_result == 0) {

				// Check for end date
				compare_result = compare_dates(data_buffer_begin[i].year, data_buffer_begin[i].month,
						data_buffer_begin[i].day, query -> end_year, query -> end_month, query -> end_day);

				if (compare_result == -1 || compare_result == 0) {

						// ENter the data to result buffer
						result_current -> year = data_buffer_begin[i].year;
						result_current -> month = data_buffer_begin[i].month;
						result_current -> day = data_buffer_begin[i].day;

						result_current ->sales_total = data_buffer_begin[i].sales_total;

						// Update data variables related to the buffer
						++result_current;
						++used_result_buffer_size;
				}
			}
		}


	}

	collapse_sale_by_date_result_buffer(used_result_buffer_size, &result_begin);

	print_sale(result_begin, used_result_buffer_size);
}

void delete_record(Query * query) {

	// Represents the result of the query to be sended to P even.
	// Initially it will be zero.
	int no_of_deleted_records = 0;
	int compare_result;
	int i;

	for (i = 0; i != used_buffer_size; ++i) {

		// Check that the record is not deleted as we don't want to delete the same record twice
		if (data_buffer_begin[i].deleted != 1) {

			// Check for start date
			compare_result = compare_dates(data_buffer_begin[i].year, data_buffer_begin[i].month, data_buffer_begin[i].day,
								query -> start_year, query -> start_month, query -> start_day);
			if (compare_result == 1 || compare_result == 0 ) {

				// Check for end date
				compare_result = compare_dates(data_buffer_begin[i].year, data_buffer_begin[i].month,
										data_buffer_begin[i].day, query -> end_year, query -> end_month, query -> end_day);
						if (compare_result == -1 || compare_result == 0) {

							// Perform deletion
							data_buffer_begin[i].deleted = 1;

							// ENter the result count
							++no_of_deleted_records;

						}
			}
		}
	}

	printf("Process: %d: Deletes Records : %d\n", my_rank, no_of_deleted_records);
}


void company_sale() {
	// These data variables represents and manages the buffer associated with the result for sale date.
	int result_buffer_size = INITIAL_RESULT_SIZE;		// Since this can not be bigger than the number of companies
	int used_result_buffer_size = 0;
	company_sale_result *result_begin;
	company_sale_result *result_current;

	int i;
	get_company_sale_result_buffer(INITIAL_RESULT_SIZE, &result_begin, &result_current);

	for (i = 0; i != used_buffer_size; ++i) {

		// First expand the buffer if it is empty
		if (used_result_buffer_size == result_buffer_size) {
			expand_company_sale_result_buffer(&result_buffer_size, &result_begin, &result_current);
		}

		// Check that the record is not deleted
		if (data_buffer_begin[i].deleted != 1) {

			// ENter the data to result buffer
			strcpy(result_current -> company_name, data_buffer_begin[i].company_name);
			result_current -> sales_total = data_buffer_begin[i].sales_total;

			result_current -> company_id = data_buffer_begin[i].company_id;

			// Update data variables related to the buffer
			++result_current;
			++used_result_buffer_size;
		}

	}

	collapse_company_sale_result_buffer(used_result_buffer_size, &result_begin);

	print_company_name(result_begin, used_result_buffer_size);
}

void print_sale(sale_by_date_result * result_begin, int used_temp_buffer_size) {
	printf("\nSale By Date Result\n");
	printf("YMD\tTotal\n");
	int i;
	for (i = 0; i != used_temp_buffer_size; ++i) {
		printf("%d/%d/%d %f\n", result_begin[i].year, result_begin[i].month, result_begin[i].day, result_begin[i].sales_total);
	}
}

void print_company_name(company_sale_result * result_begin, int used_temp_buffer_size) {
	printf("\nCompany Sale Result\n");
	printf("Company Name  Total\n");
	int i;
	for (i = 0; i != used_temp_buffer_size; ++i) {
		printf("%d %s %f\n", result_begin[i].company_id, result_begin[i].company_name, result_begin[i].sales_total);
	}
}

int compare_dates(short unsigned int lhs_year, short unsigned int lhs_month, short unsigned int lhs_day,
		short unsigned int rhs_year, short unsigned int rhs_month, short unsigned int rhs_day) {

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

