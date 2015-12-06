/*
 * DBDataDriver.h
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#ifndef DBDATADRIVER_H_
#define DBDATADRIVER_H_

#include "DBBufferManager.h"
#include "Record.h"

extern int my_rank;

int data_buffer_size;
Record * data_buffer_begin;
Record * data_buffer_current;
int empty_buffer_size;

Record *in_dummy_table;
int end;

int record_count_per_read;
int file_size;

//void set_record_count_per_read(int value);

void set_up_data_driver(int record_read_value);
void insert_data();

#endif /* DBDATADRIVER_H_ */
