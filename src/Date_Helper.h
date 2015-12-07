/*
 * Date_Helper.h
 *
 *  Created on: Dec 7, 2015
 *      Author: parth
 */

#ifndef DATE_HELPER_H_
#define DATE_HELPER_H_

static int days[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int days_in_month(int month, int year);
int is_leap(int year);

int days_between_dates(int start_year, int start_month, int start_day,
		int end_year, int end_month, int end_day);
void add_days_to_date(int year, int month, int day, int number_of_days,
		int *new_year, int *new_month, int * new_day);

#endif /* DATE_HELPER_H_ */
