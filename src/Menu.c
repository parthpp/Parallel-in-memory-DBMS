/*
 * Menu.c
 *
 *  Created on: Dec 4, 2015
 *      Author: parth
 */

#include "Menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_menu();
unsigned long get_int_input(FILE * input_stream, int input_length,int input_base );


void get_input(Query * new_query) {
	setvbuf(stdout, NULL, _IONBF, 0);
	//system("clear");
	get_menu();
	new_query -> query_id = get_int_input(stdin,1,10);

	if (new_query -> query_id == 1 || new_query -> query_id == 4) {
		return;
	} else if (new_query -> query_id == 2 || new_query -> query_id == 3) {

		printf("Enter starting year: ");
		new_query -> start_year = get_int_input(stdin,4,10);

		printf("Enter starting month: " );
		new_query -> start_month = get_int_input(stdin,2,10);


		printf("Enter starting day: " );
		new_query -> start_day = get_int_input(stdin,2,10);

		printf("Enter ending year: " );
		new_query -> end_year = get_int_input(stdin,4,10);

		printf("Enter ending month: " );
		new_query -> end_month = get_int_input(stdin,2,10);

		printf("Enter ending day: " );
		new_query -> end_day = get_int_input(stdin,2,10);
	}
}

void get_menu() {
	printf("\nParallel DBMS Main Menu\n"
			"\t1. Company Sale\n"
			"\t2. Sales By Date\n"
			"\t3. Delete By Date\n"
			"\t4. Exit\n");
}

unsigned long get_int_input(FILE * input_stream, int input_length,int input_base ) {
	++input_length;
	char buf[100];
	char *invalidIndicator;
	long choice;
	if ( fgets(buf, input_length, input_stream) != NULL) {
		if (buf[strlen(buf) - 1] != '\n' ) {

				while(getchar() != '\n') {

				}
		}
	choice = strtoul(buf, &invalidIndicator, input_base);

		return choice;
	}
	return 1;
}


