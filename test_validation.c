
#include <stdio.h>
#include <math.h>
#include "validation.h"

int main()
{
    Student s = {0};
    char message[200];

    /* Give the student a valid performance label */
    s.label = 2;

    /* Test 1: Valid student */
    s.x[0] = 3.25;
    s.x[1] = 3.50;
    s.x[9] = 5;
    s.x[10] = 85;
    s.x[11] = 90;
    s.x[12] = 1;

    if (validate_student(&s, message, sizeof(message)))
        printf("Test 1: Valid student record\n");
    else
        printf("Test 1: %s\n", message);

    /* Test 2: Invalid attendance */
    s.x[10] = 150;

    if (validate_student(&s, message, sizeof(message)))
        printf("Test 2: Unexpectedly valid\n");
    else
        printf("Test 2: %s\n", message);

    /* Test 3: Invalid CGPA */
    s.x[10] = 85;
    s.x[0] = 5.5;

    if (validate_student(&s, message, sizeof(message)))
        printf("Test 3: Unexpectedly valid\n");
    else
        printf("Test 3: %s\n", message);

    /* Test 4: Invalid performance label */
    s.x[0] = 3.25;
    s.label = 5;

    if (validate_student(&s, message, sizeof(message)))
        printf("Test 4: Unexpectedly valid\n");
    else
        printf("Test 4: %s\n", message);

    /* Test 5: Non-finite feature */
    s.label = 2;
    s.x[10] = NAN;

    if (validate_student(&s, message, sizeof(message)))
        printf("Test 5: Unexpectedly valid\n");
    else
        printf("Test 5: %s\n", message);

    return 0;
}
