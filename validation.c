
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "validation.h"

/* Feature names for error messages */
static const char *validation_names[FEATURE_COUNT] = {
    "Previous CGPA",
    "Current CGPA",
    "Introduction to Algorithms mark",
    "Theory of Computing mark",
    "Computer Networking mark",
    "Linear Algebra mark",
    "Business Communication mark",
    "Computer Networking Lab mark",
    "Algorithm Lab mark",
    "Daily study hours",
    "Attendance percentage",
    "Assignment submission rate",
    "Previous failed courses",
    "Lab practical performance",
    "Class participation"
};

/* Check whether one feature has a valid value */
int validate_feature(int index, double value)
{
    /* Check whether index is valid */
    if (index < 0 || index >= FEATURE_COUNT)
        return 0;

    /* Reject NaN and infinity */
    if (!isfinite(value))
        return 0;

    /* CGPA must be between 0 and 4 */
    if (index == 0 || index == 1)
    {
        if (value < 0 || value > 4)
            return 0;
    }

    /* Marks and percentages must be between 0 and 100 */
    else if (index >= 2 && index <= 8)
    {
        if (value < 0 || value > 100)
            return 0;
    }

    else if (index == 10 || index == 11 ||
             index == 13 || index == 14)
    {
        if (value < 0 || value > 100)
            return 0;
    }

    /* Daily study hours must be between 0 and 24 */
    else if (index == 9)
    {
        if (value < 0 || value > 24)
            return 0;
    }

    /* Previous failed courses must be a
       non-negative whole number */
    else if (index == 12)
    {
        if (value < 0 || value > 100 ||
            value != (int)value)
            return 0;
    }

    return 1;
}

/* Check the complete student record */
int validate_student(const Student *s,
                     char *error_message,
                     size_t error_size)
{
    int i;

    /* Clear the previous error message */
    if (error_message != NULL && error_size > 0)
        error_message[0] = '\0';

    /* Check whether student pointer is valid */
    if (s == NULL)
    {
        if (error_message != NULL && error_size > 0)
            snprintf(error_message, error_size,
                     "Student record is NULL");

        return 0;
    }

    /* Check all 15 features */
    for (i = 0; i < FEATURE_COUNT; i++)
    {
        if (!validate_feature(i, s->x[i]))
        {
            if (error_message != NULL && error_size > 0)
            {
                snprintf(error_message, error_size,
                         "Invalid value for %s",
                         validation_names[i]);
            }

            return 0;
        }
    }

    /* Check the performance label */
    if (s->label < 0 || s->label >= CLASS_COUNT)
    {
        if (error_message != NULL && error_size > 0)
        {
            snprintf(error_message, error_size,
                     "Invalid student performance label");
        }

        return 0;
    }

    return 1;
}
