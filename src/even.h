/*
 * even.h
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#ifndef EVEN_H_
#define EVEN_H_

#include "mpi.h"
#include "Query.h"
#include "Menu.h"

extern int my_rank;
extern int world_size;
extern const int PROCESS_ZERO;

MPI_Comm EVEN_COMMUNICATOR;
int my_even_communicator_rank;
int even_communicator_world_size;
int my_odd_partner_rank;

MPI_Datatype query_type;

void set_even_process_communicator();
void start_even_process();

#endif /* EVEN_H_ */
