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
