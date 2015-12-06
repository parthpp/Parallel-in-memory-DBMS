/*
 * DBDataDriver.c
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#include "DBDataDriver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void dummy_table();
void set_up_file_connector(int my_rank);
void split_string(char * record_string, char **record_tokens);
void get_record(char **record_token, Record * record);

void set_up_data_driver(int record_read_value) {
	record_count_per_read = record_read_value;
	set_up_file_connector(my_rank);
	data_buffer_size = 1;

	get_record_buffer(data_buffer_size, &data_buffer_begin);
	data_buffer_current = data_buffer_begin;
	used_buffer_size = 0;
}

void set_up_file_connector(int my_rank) {
	// TODO Set data buffer size & file_size
	delim = "|";
	end = 10;
	// TODO Set file descriptor and open file for reading
	dummy_table();
}

// Creates and inserts dummy values
void dummy_table() {
	in_dummy_table = (char **)malloc(sizeof(char*) * end);
	in_dummy_table[0] = "12|2014|12|2|1111|Ganga|22.20";
	in_dummy_table[1] = "12|2014|12|2|1111|Ganga|22.21";
	in_dummy_table[2] = "12|2014|12|2|1111|Ganga|22.22";
	in_dummy_table[3] = "12|2014|12|2|1111|Ganga|22.23";
	in_dummy_table[4] = "12|2014|12|2|1111|Ganga|22.24";
	in_dummy_table[5] = "12|2014|12|2|1111|Ganga|22.25";
	in_dummy_table[6] = "12|2014|12|2|1111|Ganga|22.26";
	in_dummy_table[7] = "12|2014|12|2|1111|Ganga|22.27";
	in_dummy_table[8] = "12|2014|12|2|1111|Ganga|22.28";
	in_dummy_table[9] = "12|2014|12|2|1111|Ganga|22.29";

}

// Main Insert method- This is the method called by odd process when it is time to read more data fromm file
void insert_data() {
	int i;
	char * record_tokens[7];

		for (i = 0; i != record_count_per_read; ++i) {
			// Check if we have reached the end of the file
			if (end != 0) {

				// Expand the buffer if it is full.
				if (used_buffer_size == data_buffer_size) {
					// This function will reset the values of its arguments according to the new buffer
					expand_buffer(&data_buffer_size,
							&data_buffer_begin, &data_buffer_current);
					printf("Process: %d: Expanded Buffer to : %d\n", my_rank, data_buffer_size);
				}
				// Insert values
				split_string(*in_dummy_table, record_tokens);
				get_record(record_tokens, data_buffer_current);
				fprintf(stdout,"Process: %d: Record: %d: Round id: %d: value: %f\n", my_rank, end, i, data_buffer_current -> sales_total);

				// Increment pointers
				++data_buffer_current;
				++in_dummy_table;

				// Change buffer counting values
				--end;
				++used_buffer_size;
			}
			// If reading is complete then break form the loop and simply return
			else {
				printf("Process: %d: Read Complete", my_rank);
				break;
			}
		}
}

// NOte: It is assumed that record_tokens will be have been properly allocated
// the required space to hold all the tokens before calling this method
void split_string(char * record_string, char **record_tokens) {
	// Create Duplicate
	char * s = strdup(record_string);

	// token - represents a single token
	char *token;
	// save-ptr is just a pointer required by strtok_r
	char *save_ptr;
	int i = 0;

	// Get the first token as well as set up for upcoming ones.
	token = strtok_r(s, delim, &save_ptr);

	// Get all the tokens and add them to record_tokens
	while (token != NULL) {
		record_tokens[i] = strdup(token);
		token = strtok_r(NULL, delim, &save_ptr);
		++i;
	}
}

// NOte: This method assumes the format of the data present in the string to be according to the specification of the project
// It is assumed that all the record fields will have some value. I.e null values are not considered or handled.
void get_record(char **record_token, Record * record) {
	record -> sales_id = strtol(record_token[0],NULL, 10);
	record -> year = strtol(record_token[1], NULL, 10);
	record -> month = strtol(record_token[2], NULL, 10);
	record -> day = strtol(record_token[3], NULL, 10);
	record -> company_id = strtol(record_token[4], NULL,10);
	record -> company_name = record_token[5];
	record -> company_name_length = strlen(record_token[5]);
	record -> sales_total = strtod(record_token[6], NULL);
	record ->  deleted = 0;
}
