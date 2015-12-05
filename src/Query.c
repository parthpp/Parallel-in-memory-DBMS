/*
 * Query.c
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */


#include "Query.h"


void get_MPI_Type_for_query(MPI_Datatype * query_type) {
	const int count = 7;
	int array_of_block_lengths[] = {1,1,1,1,1,1,1};
	MPI_Aint offsets[count];
	MPI_Datatype types[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
	offsets[0] = offsetof(Query, query_id);

	offsets[1] = offsetof(Query, start_year);
	offsets[2] = offsetof(Query, start_month);
	offsets[3] = offsetof(Query, start_day);

	offsets[4] = offsetof(Query, end_year);
	offsets[5] = offsetof(Query, end_month);
	offsets[6] = offsetof(Query, end_day);

	MPI_Type_create_struct(count, array_of_block_lengths, offsets, types, query_type);
	MPI_Type_commit(query_type);
}
