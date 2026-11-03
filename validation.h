
#ifndef VALIDATION_H
#define VALIDATION_H

#include "student.h"
#include <stddef.h>

/* Check whether one feature has a valid value */
int validate_feature(int index, double value);

/* Check whether an entire student record is valid */
int validate_student(const Student *s,
                     char *error_message,
                     size_t error_size);

#endif
