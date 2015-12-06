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
	// New buffer will be double the size of the previous one
	int new_no_of_elements = (*no_of_elements) * 2;

	// Create a new buffer, and assign buffer_begin to point to the new buffer
	// realloc will copy the values form the old buffer to the new buffer and return the starting address of the new buffer on success
	*buffer_begin = (Record *)realloc(*buffer_begin, (new_no_of_elements * sizeof(Record)));

	// Check if the buffer was successfully created or not
	if (*buffer_begin == NULL) {
		fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
		//TODO
				//terminate_system();
	}

	// Assign current buffer_current to point to the next empty slot where the next value can be stored.
	*buffer_current = ((*buffer_begin) + (*no_of_elements));

	// Change the size of the buffer to be according to the new buffer
	*no_of_elements = new_no_of_elements;
}

