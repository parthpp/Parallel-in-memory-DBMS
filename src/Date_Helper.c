/*
 * Date_Helper.c
 *
 *  Created on: Dec 7, 2015
 *      Author: parth
 */


#include "Date_Helper.h"
#include <time.h>

int days_between_dates(int start_year, int start_month, int start_day,
		int end_year, int end_month, int end_day) {
	--start_month;		// Index for algorithm starts at index 0.
	--end_month;
	int result = 0;
	int i, j;
	if (start_year == end_year) {
		if (start_month == end_month) {
			// Inclusive of start and end date
			result = end_day - start_day + 1;
			return result;
		} else {
			for (i = start_month + 1; i != end_month; ++i) {
				result += days_in_month(i, start_year);
			}

			// Start date inclusive
			result += days_in_month(start_month, start_year) - start_day + 1;
			result += end_day;
			return result;
		}
	} else {
		for (j = start_year + 1; j != end_year; ++j) {
			if (is_leap(j) == 1) {
				result += 366;
			} else {
				result += 365;
			}
		}
		// Calculate days in start year
		for (i = start_month + 1; i != 12; ++i) {
			result += days_in_month(i, start_year);
		}

		result += days_in_month(start_month, start_year) - start_day + 1;

		// Add days from end_year
		for (i = 0; i != end_month; ++i) {
			result += days_in_month(i, end_year);
		}

		result += end_day;
		return result;
	}
}

void add_days_to_date(int year, int month, int day, int number_of_days,
		int *new_year, int *new_month, int * new_day) {
	struct tm t ={};
	time_t rawtime;
	t.tm_year = year - 1900;
	t.tm_mon  = month - 1;
	t.tm_mday = day;

	t.tm_mday += number_of_days;
	rawtime = mktime(&t);

	*new_day = t.tm_mday;
	*new_month = t.tm_mon + 1;
	*new_year = t.tm_year + 1900;


}

int days_in_month(int month, int year) {
	int leap = is_leap(year);

	if (month < 0 || month > 11)
		return -1;

	return days[leap][month];
}

int is_leap(int year) {

	if (year % 400 == 0) {
		return 1;
	} else if (year % 100 == 0) {
		return 0;
	} else if (year % 4 == 0) {
		return 1;
	} else {
		return 0;
	}
}
