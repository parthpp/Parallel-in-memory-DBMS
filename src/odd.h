/*
 * odd.h
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#ifndef ODD_H_
#define ODD_H_

#include "mpi.h"
#include <signal.h>

extern int my_rank;
extern int world_size;
extern const int PROCESS_ZERO;
extern int my_even_communicator_rank;

int my_even_partner_rank;
volatile sig_atomic_t read_pending;
MPI_Datatype query_type;

void set_odd_process_structures(int read_record_per_count);
void start_odd_process(int read_record_per_count);
void signal_handler(int read_signal);
void terminate_odd();


#endif /* ODD_H_ */
