/*
 * Query_Result.c
 *
 *  Created on: Dec 6, 2015
 *      Author: parth
 */


#include "Query_Result.h"
#include <stdlib.h>
#include <stdio.h>

void get_company_sale_result_buffer(int no_of_elements, company_sale_result **buffer, company_sale_result **buffer_current) {
	*buffer = (company_sale_result *) malloc(no_of_elements * sizeof(company_sale_result));
	if (*buffer == NULL && (no_of_elements != 0)) {
		fprintf(stderr, "A buffer creation process failed at : process: %d\n", my_rank);
	} else {
		*buffer_current = *buffer;
	}

	printf("Current Buffer Size for company sale: %d\n", no_of_elements);
}

void expand_company_sale_result_buffer(int *no_of_elements, company_sale_result **buffer_begin,
		company_sale_result ** buffer_current) {
	int new_no_of_elements = (*no_of_elements) * 2;
	*buffer_begin = (company_sale_result *) realloc(*buffer_begin,
			(new_no_of_elements * sizeof(company_sale_result)));
	if (*buffer_begin == NULL && (new_no_of_elements != 0)) {
			fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
			//TODO
			//terminate_system();
	}
	printf("Buffer for company sale Expanded from %d to %d\n", *no_of_elements, new_no_of_elements);

	*buffer_current = ((*buffer_begin) + (*no_of_elements));
	*no_of_elements = new_no_of_elements;


}

void get_sale_by_date_result_buffer(int no_of_elements, sale_by_date_result **buffer, sale_by_date_result **buffer_current) {
	*buffer = (sale_by_date_result *) malloc(no_of_elements * sizeof(sale_by_date_result));
	if (*buffer == NULL && (no_of_elements != 0)) {
		fprintf(stderr, "A buffer creation process failed at : process: %d\n", my_rank);
	} else {
		*buffer_current = *buffer;
	}

	printf("Current Buffer Size for sale by date: %d\n", no_of_elements);

}

void expand_sale_by_date_result_buffer(int *no_of_elements, sale_by_date_result **buffer_begin,
		sale_by_date_result ** buffer_current) {
	int new_no_of_elements = (*no_of_elements) * 2;
	*buffer_begin = (sale_by_date_result *) realloc(*buffer_begin,
			(new_no_of_elements * sizeof(sale_by_date_result)));
	if (*buffer_begin == NULL && (new_no_of_elements != 0)) {
			fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
			//TODO
			//terminate_system();
	}
	printf("Buffer For Sale Expanded from %d to %d\n", *no_of_elements, new_no_of_elements);

	*buffer_current = ((*buffer_begin) + (*no_of_elements));
	*no_of_elements = new_no_of_elements;
	printf("Just for build");
}

void collapse_sale_by_date_result_buffer(int new_no_of_elements, sale_by_date_result **buffer_begin) {
	*buffer_begin = (sale_by_date_result *) realloc(*buffer_begin, (new_no_of_elements * sizeof(sale_by_date_result)));
	if (*buffer_begin == NULL && (new_no_of_elements != 0)) {
				fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
				//TODO
				//terminate_system();
		}

	printf("Buffer for sale by date collapsed to %d", new_no_of_elements);
}

void collapse_company_sale_result_buffer(int new_no_of_elements, company_sale_result **buffer_begin) {
	*buffer_begin = (company_sale_result *) realloc(*buffer_begin, (new_no_of_elements * sizeof(company_sale_result)));
	if (*buffer_begin == NULL && (new_no_of_elements != 0)) {
				fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
				//TODO
				//terminate_system();
		}

	printf("Buffer for company sale collapsed to %d", new_no_of_elements);

}

void get_company_sale_result_MPI_Type(MPI_Datatype * company_sale_result_type) {
	const int count = 3;
	int array_of_block_lengths[] = {1,100,1};
	MPI_Aint offsets[count];
	MPI_Datatype types[] = {MPI_UNSIGNED_SHORT, MPI_CHAR, MPI_DOUBLE};
	offsets[0] = offsetof(company_sale_result, company_id);

	offsets[1] = offsetof(company_sale_result, company_name);
	offsets[2] = offsetof(company_sale_result, sales_total);

	MPI_Type_create_struct(count, array_of_block_lengths, offsets, types, company_sale_result_type);
	MPI_Type_commit(company_sale_result_type);
}
