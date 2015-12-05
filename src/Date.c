/*
 * Date.c
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#include "Date.h"

void get_MPI_Type_for_date_range(MPI_Datatype *date_range_type) {
	const int count = 3;
	int array_of_block_lengths[] = {1,1,1};
	MPI_Aint offsets[count];
	MPI_Datatype types[] = {MPI_INT, MPI_INT, MPI_INT};
	offsets[0] = offsetof(Date, year);
	offsets[1] = offsetof(Date, month);
	offsets[2] = offsetof(Date, day);

	MPI_Type_create_struct(count, array_of_block_lengths, offsets, types, date_range_type);
	MPI_Type_commit(date_range_type);
}
