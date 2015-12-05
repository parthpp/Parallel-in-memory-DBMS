/*
 * Record.h
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#ifndef RECORD_H_
#define RECORD_H_

typedef struct {
	unsigned int sales_id;

	int year;
	int month;
	int day;

	unsigned int company_id;

	char * company_name;
	unsigned int company_name_length;

	float sales_total;

	unsigned short deleted;
}Record;

#endif /* RECORD_H_ */
