/*
 * Query_Processor.c
 *
 *  Created on: Dec 6, 2015
 *      Author: parth
 */

#include "Query_Processor.h"

void company_sale();
void sale_by_date(Query *query);
void delete_record(Query *query);
void terminate_system();

void process_query(Query * query) {

	if (query -> query_id == 1) {
		company_sale();
	} else if (query -> query_id == 2) {
		sale_by_date(query);
	} else if (query -> query_id == 3) {
		delete_record(query);
	} else if (query -> query_id == 4) {
		terminate_system();
	}
}

void sale_by_date(Query *query) {
	// These data variables represents and manages the buffer associated with the result for sale date.
	int result_buffer_size = INITIAL_RESULT_SIZE;
	int used_result_buffer_size = 0;
	sale_by_date_result *result_begin;
	sale_by_date_result *result_current;

	int i;
	get_sale_by_date_result(INITIAL_RESULT_SIZE, &result_begin, &result_current);

	for (i = 0; i != used_buffer_size; ++i) {

		// First expand the buffer if it is empty
		if (used_result_buffer_size == result_buffer_size) {
			expand_sale_by_date_result_buffer(&result_buffer_size, &result_begin, &result_current);
		}

		// Check that the record is not deleted
		if (data_buffer_begin[i].deleted != 1) {

			// Check for start date
			if ((data_buffer_begin[i].year >= query -> start_year)  &&
							(data_buffer_begin[i].month >= query -> start_month) &&
							(data_buffer_begin[i].day >= query -> start_day) ) {

				// Check for end date
						if ((data_buffer_begin[i].year <= query -> end_year)  &&
										(data_buffer_begin[i].month <= query -> end_month) &&
										(data_buffer_begin[i].day <= query -> end_day) ) {

							// ENter the data to result buffer
							result_current -> year = data_buffer_begin[i].year;
							result_current -> month = data_buffer_begin[i].month;
							result_current -> day = data_buffer_begin[i].day;

							// Update data variables related to the buffer
							++result_current;
							++used_result_buffer_size;
						}
			}
		}
	}
}
