/*
 * Query_Result.h
 *
 *  Created on: Dec 6, 2015
 *      Author: parth
 */

#ifndef QUERY_RESULT_H_
#define QUERY_RESULT_H_

extern int my_rank;		// USed while printing error messgaes, if error occurs.
typedef struct {
	unsigned long company_id;
	char company_name[100];
	double sales_total;
} company_sale_result;

typedef struct {
	short unsigned int year;
	short unsigned int month;
	short unsigned int day;

	double sales_total;
} sale_by_date_result;

typedef int delete_record_result;

void get_company_sale_result_buffer(int no_of_elements, company_sale_result **buffer, company_sale_result **buffer_current);
void expand_company_sale_result_buffer(int *no_of_elements, company_sale_result **buffer_begin,
		company_sale_result ** buffer_current);

void get_sale_by_date_result_buffer(int no_of_elements, sale_by_date_result **buffer, sale_by_date_result **buffer_current);
void expand_sale_by_date_result_buffer(int *no_of_elements, sale_by_date_result **buffer_begin,
		sale_by_date_result ** buffer_current);

void collapse_sale_by_date_result_buffer(int new_no_of_elements, sale_by_date_result **buffer_begin);
void collapse_company_sale_result_buffer(int new_no_of_elements, company_sale_result **buffer_begin);

#endif /* QUERY_RESULT_H_ */
