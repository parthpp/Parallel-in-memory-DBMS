/*
 * DBDataDriver.c
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#include "DBDataDriver.h"
#include <string.h>
#include <stdio.h>

void dummy_table();
void set_up_file_connector(int my_rank);
void set_up_data_driver(int record_read_value) {
	record_count_per_read = record_read_value;
	set_up_file_connector(my_rank);
	data_buffer_size = 1;

	get_record_buffer(data_buffer_size, &data_buffer_begin);
	data_buffer_current = data_buffer_begin;
	empty_buffer_size = data_buffer_size;
}

void set_up_file_connector(int my_rank) {
	// TODO Set data buffer size & file_size
	file_size = 10;

	// TODO Set file descriptor and open file for reading
	dummy_table();
}

void dummy_table() {
	int i;
	Record *dummy_table_iterator;
	get_record_buffer(file_size, &in_dummy_table);
	dummy_table_iterator = in_dummy_table;
	end = file_size;
	for (i = 0; i != file_size; ++i) {
		dummy_table_iterator-> sales_id = i;
		dummy_table_iterator -> year = i + 1;
		dummy_table_iterator -> month = i + my_rank;
		dummy_table_iterator -> company_id = i + 3;
		dummy_table_iterator -> company_name = "data";
		dummy_table_iterator -> company_name_length = i +strlen(dummy_table_iterator -> company_name);
		dummy_table_iterator -> deleted = 0;
		++dummy_table_iterator;
	}
}

void insert_data() {
	int i;

		for (i = 0; i != record_count_per_read; ++i) {
			if (end != 0) {

				if (empty_buffer_size == 0) {
					expand_buffer(&data_buffer_size, &empty_buffer_size,
							&data_buffer_begin, &data_buffer_current);
					printf("Process: %d: Expanded Buffer to : %d\n", my_rank, data_buffer_size);
				}
				*data_buffer_current = *in_dummy_table;
				printf("Process: %d: Record: %d: Round id: %d: value: %d\n", my_rank, end, i, data_buffer_current -> month);
				++data_buffer_current;
				++in_dummy_table;
				--end;
				--empty_buffer_size;
			} else {
				printf("Process: %d: Read Complete", my_rank);
				break;
			}
		}
}
