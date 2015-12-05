/*
 * query.h
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#ifndef QUERY_H_
#define QUERY_H_

#include "mpi.h"
#include "Date.h"


typedef struct {
	int query_id;

	Date start_date;
	Date end_date;
} Query;

MPI_Datatype date_range_type;
MPI_Datatype query_type;

void create_MPI_Type_for_query();

void get_MPI_Type_for_query();

//TODO free mpi type for query
//void free_MPI_Type_for_query();


#endif /* QUERY_H_ */
