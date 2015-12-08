/*
 * Shared_Memory.h
 *
 *  Created on: Dec 8, 2015
 *      Author: parth
 */

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

#include "Query_Result.h"

void send_sbd_result(sale_by_date_result *result, int result_size) ;
void write_sbd_to_sm(sale_by_date_result *result, int result_size, sale_by_date_result ** sm_ptr);

void read_sbd_from_sm(sale_by_date_result *sm_ptr, int result_size, sale_by_date_result * result);
void receive_sbd_from_sm(sale_by_date_result *result, int result_size);


#endif /* SHARED_MEMORY_H_ */
