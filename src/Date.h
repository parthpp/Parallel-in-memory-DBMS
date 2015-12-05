/*
 * Date.h
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#ifndef DATE_H_
#define DATE_H_

#include "mpi.h"

typedef struct {
	int year;
	int month;
	int day;
} Date;

void get_MPI_Type_for_date_range(MPI_Datatype *date_range_type);

#endif /* DATE_H_ */
