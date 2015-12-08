/*
 * Shared_Memory.c
 *
 *  Created on: Dec 8, 2015
 *      Author: parth
 */

#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "Shared_Memory.h"

void send_sbd_result(sale_by_date_result *result, int result_size) {
	int my_id = my_rank;
	key_t key;
	int shmid;
	int shmflg;
	int size_in_bytes;
	void * shared_memory = (void *)0;
	int shmflag;

	sale_by_date_result *sm_ptr;

	size_in_bytes = result_size * sizeof(sale_by_date_result);
	shmflag = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
	key = (key_t)(123 + my_id);
	printf("ftok() successful. key = %d\n", key );


	shmid = shmget(key, size_in_bytes, shmflag);
	if (shmid == -1) {
		perror("Creation of shared memory failed");
	}

	printf("Process: %d is attached to shared mem with key %d\n", my_rank, key);

	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1)
	{
		printf ("shmat failed\n");
		exit (EXIT_FAILURE);
	}

	sm_ptr = (sale_by_date_result *) shared_memory;
	write_sbd_to_sm(result, result_size, &sm_ptr);

	printf("Write Complete\n");
	if (shmdt (shared_memory) == -1)
	{
		printf ("shmdt failed\n ");
		exit (EXIT_FAILURE);
	}
}

void write_sbd_to_sm(sale_by_date_result *result, int result_size, sale_by_date_result ** sm_ptr){
	int i = result_size;
	for (i = 0; i != result_size; ++i) {
		(*sm_ptr)[i].year = result[i].year;
		(*sm_ptr)[i].month = result[i].month;
		(*sm_ptr)[i].day = result[i].day;
		(*sm_ptr)[i].sales_total = result[i].sales_total;
	}
}

void receive_sbd_from_sm(sale_by_date_result *result, int result_size) {
	int my_id = my_rank+1;
	key_t key;
	int shmid;
	int shmflg;
	int size_in_bytes;
	void * shared_memory = (void *)0;
	int shmflag;

	sale_by_date_result *sm_ptr;

	size_in_bytes = result_size * sizeof(sale_by_date_result);
	shmflag = 0;

	key = (key_t)(123 + my_id);
	printf("ftok() successful. key = %d\n", key );


	shmid = shmget(key, size_in_bytes, shmflag);
	if (shmid == -1) {
		perror("Creation of shared memory failed");
	}

	printf("Process: %d is attached to shared mem with key %d\n", my_rank, key);

	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1)
	{
		printf ("shmat failed\n");
		exit (EXIT_FAILURE);
	}

	sm_ptr = (sale_by_date_result *) shared_memory;
	read_sbd_from_sm(sm_ptr, result_size, result);

	printf("read Complete\n");

	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		printf ("shmcl(IPC_RMID) failed\n");
		exit (EXIT_FAILURE);
	}
}

void read_sbd_from_sm(sale_by_date_result *sm_ptr, int result_size, sale_by_date_result * result) {
	int i = result_size;
	for (i = 0; i != result_size; ++i) {
		result[i].year = sm_ptr[i].year;
		result[i].month = sm_ptr[i].month;
		result[i].day = sm_ptr[i].day;
		result[i].sales_total = sm_ptr[i].sales_total;
	}
}

void send_cs_result(company_sale_result *result, int result_size) {
	int my_id = my_rank;
	key_t key;
	int shmid;
	int shmflg;
	int size_in_bytes;
	void * shared_memory = (void *)0;
	int shmflag;

	company_sale_result *sm_ptr;

	size_in_bytes = result_size * sizeof(company_sale_result);
	shmflag = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;
	key = (key_t)(123 + my_id);
	printf("ftok() successful. key = %d\n", key );


	shmid = shmget(key, size_in_bytes, shmflag);
	if (shmid == -1) {
		perror("Creation of shared memory failed");
	}

	printf("Process: %d is attached to shared mem with key %d\n", my_rank, key);

	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1)
	{
		printf ("shmat failed\n");
		exit (EXIT_FAILURE);
	}

	sm_ptr = (company_sale_result *) shared_memory;
	write_cs_to_sm(result, result_size, &sm_ptr);

	printf("Write Complete\n");
	if (shmdt (shared_memory) == -1)
	{
		printf ("shmdt failed\n ");
		exit (EXIT_FAILURE);
	}
}

void write_cs_to_sm(company_sale_result *result, int result_size, company_sale_result ** sm_ptr){
	int i = result_size;
	for (i = 0; i != result_size; ++i) {
		(*sm_ptr)[i].company_id = result[i].company_id;
		strcpy((*sm_ptr)[i].company_name, result[i].company_name);
		(*sm_ptr)[i].sales_total = result[i].sales_total;
	}
}

void receive_cs_from_sm(company_sale_result *result, int result_size) {
	int my_id = my_rank+1;
	key_t key;
	int shmid;
	int shmflg;
	int size_in_bytes;
	void * shared_memory = (void *)0;
	int shmflag;

	company_sale_result *sm_ptr;

	size_in_bytes = result_size * sizeof(company_sale_result);
	shmflag = 0;

	key = (key_t)(123 + my_id);
	printf("ftok() successful. key = %d\n", key );


	shmid = shmget(key, size_in_bytes, shmflag);
	if (shmid == -1) {
		perror("Creation of shared memory failed");
	}

	printf("Process: %d is attached to shared mem with key %d\n", my_rank, key);

	shared_memory = shmat(shmid, (void *)0, 0);

	if (shared_memory == (void *)-1)
	{
		printf ("shmat failed\n");
		exit (EXIT_FAILURE);
	}

	sm_ptr = (company_sale_result *) shared_memory;
	read_cs_from_sm(sm_ptr, result_size, result);

	printf("read Complete\n");

	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		printf ("shmcl(IPC_RMID) failed\n");
		exit (EXIT_FAILURE);
	}
}

void read_cs_from_sm(company_sale_result *sm_ptr, int result_size, company_sale_result * result) {
	int i = result_size;
	for (i = 0; i != result_size; ++i) {
		result[i].company_id = sm_ptr[i].company_id;
		strcpy(result[i].company_name, sm_ptr[i].company_name);
		result[i].sales_total = sm_ptr[i].sales_total;
	}
}

