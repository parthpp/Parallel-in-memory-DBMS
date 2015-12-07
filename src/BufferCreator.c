/*
 * BufferCreator.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#include "BufferCreator.h"
#include <stdio.h>
#include <stdlib.h>

extern int my_rank;
void get_int_buffer(int size, int **buffer) {
	*buffer = (int *) malloc(size * sizeof(int)) ;

	if (*buffer == NULL) {
		fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
		//TODO
		//terminate_system();
	}
}

void get_unsigned_long_buffer (int size, unsigned long **buffer) {
	*buffer = (unsigned long *) malloc(size * sizeof(unsigned long)) ;

		if (*buffer == NULL) {
			fprintf(stderr, "Buffer Creation error at process: %d\nSystem will now terminate\n", my_rank);
			//TODO
			//terminate_system();
		}
}
