/*
 * DBBufferManager.c
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#include "DBBufferManager.h"
#include <stdio.h>
#include <stdlib.h>

void get_record_buffer(int no_of_elements, Record **new_buffer) {
	*new_buffer = (Record *) malloc(no_of_elements * sizeof(Record)) ;

	if (*new_buffer == NULL) {
		fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
		//TODO
		//terminate_system();
	}

}
void expand_buffer(int *no_of_elements, Record **buffer_begin, Record **buffer_current) {
	int new_no_of_elements = (*no_of_elements) * 2;
	*buffer_begin = (Record *)realloc(*buffer_begin, (new_no_of_elements * sizeof(Record)));
	*buffer_current = ((*buffer_begin) + (*no_of_elements));

//	int i;
//	Record *old_buffer_iterator = *buffer_begin;
//	Record *new_buffer_iterator;
//	Record *temp = *buffer_begin;
//
//	get_record_buffer(new_no_of_elements, buffer_begin);
//
//	new_buffer_iterator = *buffer_begin;
//	for (i = 0; i != *no_of_elements; ++i) {
//		*new_buffer_iterator = *old_buffer_iterator;
//		++old_buffer_iterator;
//		++new_buffer_iterator;
//	}
//	*buffer_current = new_buffer_iterator;
	*no_of_elements = new_no_of_elements;
	//*no_of_used_space = new_no_of_elements/2;
	//free(temp);
}

