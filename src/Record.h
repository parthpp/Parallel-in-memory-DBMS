/*
 * Record.h
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#ifndef RECORD_H_
#define RECORD_H_

#include "Date.h"
#include "CString.h"

typedef struct {
	unsigned int sales_id;
	Date date;

	unsigned int company_id;
	CString company_name;

	float sales_total;
}Record;

#endif /* RECORD_H_ */
