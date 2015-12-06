/*
 * DBBufferManager.h
 *
 *  Created on: Dec 5, 2015
 *      Author: parth
 */

#ifndef DBBUFFERMANAGER_H_
#define DBBUFFERMANAGER_H_

#include "Record.h"

extern int my_rank;

void get_record_buffer(int size, Record **new_buffer);
void expand_buffer(int *no_of_elements, int *no_of_empty_space, Record **buffer_begin, Record **buffer_current);
#endif /* DBBUFFERMANAGER_H_ */
