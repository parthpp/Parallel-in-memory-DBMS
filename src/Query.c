/*
 * Query.c
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */


#include "Query.h"

void create_MPI_type_for_query() {
	get_MPI_Type_for_date_range();
	get_MPI_Type_for_query();
}

void get_MPI_Type_for_date_range() {
	const int count = 3;
	int array_of_block_lengths = {1,1,1};
	MPI_Aint offsets[count];
	MPI_Datatype types[] = {MPI_INT, MPI_INT, MPI_INT};
	offsets[0] = offsetof(Date, year);
	offsets[1] = offsetof(Date, month);
	offsets[2] = offsetof(Date, day);

	MPI_Type_create_struct(count, array_of_block_lengths, offsets, types, date_range_type);
	MPI_Type_commit(date_range_type);
}


void get_MPI_Type_for_query() {
	const int count = 3;
	int array_of_block_lengths[] = {1,1,1};
	MPI_Aint offsets[count];
	MPI_Datatype types[] = {MPI_INT, date_range_type, date_range_type};
	offsets[0] = offsetof(Query, query_id);
	offsets[1] = offsetof(Query, start_date);
	offsets[2] = offsetof(Query, end_date);

	MPI_Type_create_struct(count, array_of_block_lengths, offsets, types, query_type);
	MPI_Type_commit(query_type);
}
