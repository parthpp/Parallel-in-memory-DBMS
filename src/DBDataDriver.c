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
#include <errno.h>
#include <sys/stat.h>

//TODO These headers are for testing only
#include "Query.h"
#include "Query_Processor.h"

char *file_names[5] = {"data0.txt", "data1.txt", "data2.txt", "data3.txt", "data4.txt"};

//void dummy_table();
void set_up_file_connector();
void split_string(char * record_string, char **record_tokens);
void get_record(char **record_token, Record * record);
void get_file_details();
void read_record(char **record_string);
void get_file_details();

void set_up_data_driver(int record_read_value) {
	record_count_per_read = record_read_value;
	set_up_file_connector();
	data_buffer_size = 1500;

	get_record_buffer(data_buffer_size, &data_buffer_begin);
	data_buffer_current = data_buffer_begin;
	used_buffer_size = 0;
}

void set_up_file_connector() {
	// TODO Set data buffer size & file_size
	delim = "|/";
	end = 0;
	//TODO Set file descriptor and open file for reading
	RAW_DATA_BUFFER_SIZE = 1000;
	//dummy_table();
	get_file_details();


}

// Creates and inserts dummy values
//void dummy_table() {
//	in_dummy_table = (char **)malloc(sizeof(char*) * end);
//	in_dummy_table[0] = "12000|2013|12|17|1111|Ganga|22.20";
//	in_dummy_table[1] = "12001|2014|1|2|1111|Ganga|22.21";
//	in_dummy_table[2] = "12002|2014|2|2|1111|Ganga|22.22";
//	in_dummy_table[3] = "12003|2014|5|8|1111|Ganga|22.23";
//	in_dummy_table[4] = "12004|2015|1|10|1111|Ganga|22.24";
//	in_dummy_table[5] = "12005|2015|8|6|1111|Ganga|22.25";
//	in_dummy_table[6] = "12006|2015|8|31|1111|Ganga|22.26";
//	in_dummy_table[7] = "12007|2015|9|26|1111|Ganga|22.27";
//	in_dummy_table[8] = "12008|2015|11|2|1111|Ganga|22.28";
//	in_dummy_table[9] = "12009|2015|12|17|1111|Ganga|22.29";
//
//}

// Main Insert method- This is the method called by odd process when it is time to read more data fromm file
void insert_data() {
	int i;
	Query company_sale;
	Query sale_by_date;
	Query deleted;
	char * record_tokens[7];
	char *record_string;

		for (i = 0; i != record_count_per_read; ++i) {
			// Check if we have reached the end of the file
			if (end != 1) {

				// Expand the buffer if it is full.
				if (used_buffer_size == data_buffer_size) {
					// This function will reset the values of its arguments according to the new buffer
					expand_buffer(&data_buffer_size,
							&data_buffer_begin, &data_buffer_current);
					printf("Process: %d: Expanded Buffer to : %d\n", my_rank, data_buffer_size);
				}
				// Insert values
				read_record(&record_string);
				split_string(record_string, record_tokens);
				get_record(record_tokens, data_buffer_current);
				free(record_string);
			//	fprintf(stdout,"Process: %d: Record: %d: Round id: %d: value: %f\n", my_rank, end, i, data_buffer_current -> sales_total);

				// Increment pointers
				++data_buffer_current;
				//++in_dummy_table;

				// Change buffer counting values
				//--end;
				++used_buffer_size;
			}
			// If reading is complete then break form the loop and simply return
			else {
				printf("Process: %d: Read Complete", my_rank);
				if (my_rank == 3) {
					company_sale.query_id = 1;
					process_query(&company_sale);

					sale_by_date.query_id = 2;
					sale_by_date.start_year = 2013;
					sale_by_date.start_month = 12;
					sale_by_date.start_day = 17;
					sale_by_date.end_year = 2015;
					sale_by_date.end_month = 8;
					sale_by_date.end_day = 31;
					process_query(&sale_by_date);

					deleted.query_id = 3;
					deleted.start_year = 2013;
					deleted.start_month = 12;
					deleted.start_day = 17;
					deleted.end_year = 2015;
					deleted.end_month = 8;
					deleted.end_day = 31;
					process_query(&deleted);

					company_sale.query_id = 1;
					process_query(&company_sale);

					sale_by_date.query_id = 2;
					sale_by_date.start_year = 2013;
					sale_by_date.start_month = 12;
					sale_by_date.start_day = 17;
					sale_by_date.end_year = 2015;
					sale_by_date.end_month = 8;
					sale_by_date.end_day = 31;
					process_query(&sale_by_date);

				}
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
	record -> sales_id = strtoul(record_token[0],NULL, 10);

	//TODO COnversion is through long
	record -> year = strtol(record_token[1], NULL, 10);
	record -> month = strtol(record_token[2], NULL, 10);
	record -> day = strtol(record_token[3], NULL, 10);
	record -> company_id = strtoul(record_token[4], NULL,10);

	strcpy(record -> company_name, record_token[5]);
	record -> sales_total = strtod(record_token[6], NULL);
	record ->  deleted = 0;
}

void get_file_details() {
	struct stat file_structure;
	int error_check ;
	errno = 0;

	file_handler = fopen(file_names[my_even_partner_rank], "rb");

	if (file_handler == NULL) {
		fprintf(stderr, "Process: %d : An error happened while attempting to open the file %s : "
				, my_rank, file_names[my_even_partner_rank]);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}
	error_check = stat(file_names[my_even_partner_rank], &file_structure);
	if (file_structure.st_size < 0 ) {
		file_size = 0;
	} else {
		file_size = file_structure.st_size;
	}
}

void read_record(char **record_string){
	int length;
	fgets(raw_data, RAW_DATA_BUFFER_SIZE, file_handler);
	if (feof(file_handler)) {
		end = 1;
	}

	length = strlen(raw_data);
	if (raw_data[length -1] == '\n') {
		--length;
		raw_data[length] = '\0';

	}
	*record_string = (char *)malloc((length+1) * sizeof(char));
	strcpy(*record_string, raw_data);
}
