/*
 * even.h
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#ifndef EVEN_H_
#define EVEN_H_

#include "mpi.h"

extern int my_rank;
extern int world_size;

MPI_Comm EVEN_COMMUNICATOR;
int my_even_communicator_rank;
int even_communicator_world_size;

void set_even_process_communicator();
void start_even_process();

#endif /* EVEN_H_ */
