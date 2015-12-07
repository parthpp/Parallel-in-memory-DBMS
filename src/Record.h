/*
 * Record.h
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#ifndef RECORD_H_
#define RECORD_H_

typedef struct {
	unsigned long sales_id;

	short unsigned int year;
	short unsigned int month;
	short unsigned int day;

	unsigned long company_id;

	char company_name[100];

	double sales_total;

	short unsigned int deleted;
}Record;

#endif /* RECORD_H_ */
