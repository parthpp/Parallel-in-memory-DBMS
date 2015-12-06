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

	short unsigned int start_year;
	short unsigned int start_month;
	short unsigned int start_day;

	short unsigned int end_year;
	short unsigned int end_month;
	short unsigned int end_day;
} Query;


void get_MPI_Type_for_query(MPI_Datatype *query_type);

//TODO free mpi type for query
//void free_MPI_Type_for_query();


#endif /* QUERY_H_ */
