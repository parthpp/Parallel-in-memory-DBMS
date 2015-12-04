/*
 * odd.h
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#ifndef ODD_H_
#define ODD_H_

#include "mpi.h"

extern int my_rank;
extern int world_size;
extern const int PROCESS_ZERO;
extern int my_even_communicator_rank;

int my_even_partner_rank;

void start_odd_process();



#endif /* ODD_H_ */
