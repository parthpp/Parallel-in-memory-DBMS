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
#include <stdio.h>

extern int my_rank;

// Buffer related variables
int data_buffer_size;				// Size of Current Buffer
Record * data_buffer_begin;			// POinter to the beigining i.e the first element of the buffer
Record * data_buffer_current;		// Pointer the next empty location in the buffer(i.e place where next element will go)
int used_buffer_size;				// number of elements in the buffer

char * delim;						// Delimeter used while decoing the strings read from file

FILE *file_handler;
int file_size;
char raw_data[1000];
int RAW_DATA_BUFFER_SIZE;
extern int my_even_partner_rank;		// Needed to retrive file name
int end;
//// Dummy record data
//char **in_dummy_table;
//int end;

int record_count_per_read;			// Random value generated between read_max and 10


void set_up_data_driver(int record_read_value);
void insert_data();

#endif /* DBDATADRIVER_H_ */
