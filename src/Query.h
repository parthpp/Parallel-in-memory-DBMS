/*
 * query.h
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#ifndef QUERY_H_
#define QUERY_H_

#include "mpi.h"


typedef struct {
	int query_id;

	int start_year;
	int start_month;
	int start_day;

	int end_year;
	int end_month;
	int end_day;
} Query;


void get_MPI_Type_for_query(MPI_Datatype *query_type);

//TODO free mpi type for query
//void free_MPI_Type_for_query();


#endif /* QUERY_H_ */
