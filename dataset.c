#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"

const char *feature_names[FEATURE_COUNT] = {
    "previous_cgpa",
    "current_cgpa",
    "introduction_to_algorithms_mark",
    "theory_of_computing_mark",
    "computer_networking_mark",
    "linear_algebra_mark",
    "business_communication_mark",
    "computer_networking_lab_mark",
    "algorithm_lab_mark",
    "daily_study_hours",
    "attendance_percentage",
    "assignment_submission_rate",
    "previous_failed_courses",
    "lab_practical_performance",
    "class_participation"
};

const char *class_names[CLASS_COUNT] = {
    "At Risk", "Average", "Good", "Excellent"
};

static void trim(char *s)
{
    size_t n;
    while (*s && isspace((unsigned char)*s)) {
        memmove(s, s + 1, strlen(s));
    }
    n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) {
        s[--n] = '\0';
    }
    /* Remove a possible UTF-8 BOM at the beginning of the first header. */
    if ((unsigned char)s[0] == 0xEF &&
        (unsigned char)s[1] == 0xBB &&
        (unsigned char)s[2] == 0xBF) {
        memmove(s, s + 3, strlen(s + 3) + 1);
    }
}

int class_from_name(const char *name)
{
    char temp[80];
    size_t i, n = strlen(name);
    if (n >= sizeof(temp)) return -1;
    strcpy(temp, name);
    trim(temp);

    for (i = 0; i < CLASS_COUNT; i++) {
        if (strcmp(temp, class_names[i]) == 0) return (int)i;
    }
    return -1;
}

/*
 * This loader finds columns by their header names, so the CSV can contain
 * extra columns and the column order can vary. It expects ordinary CSV fields
 * without commas embedded inside quoted values; the supplied student datasets
 * follow that format.
 */
static char *copy_string(const char *source)
{
    size_t length = strlen(source) + 1;
    char *copy = (char *)malloc(length);
    if (copy) memcpy(copy, source, length);
    return copy;
}

int load_csv(const char *filename, Student students[], int capacity)
{
    FILE *fp = fopen(filename, "r");
    char line[MAX_LINE];
    char *headers[100];
    int header_count = 0, feature_col[FEATURE_COUNT], label_col = -1;
    int count = 0, i;

    if (!fp) {
        printf("Error: cannot open file '%s'.\n", filename);
        return -1;
    }

    if (!fgets(line, sizeof(line), fp)) {
        printf("Error: CSV file is empty.\n");
        fclose(fp);
        return -1;
    }

    {
        char *token = strtok(line, ",\r\n");
        while (token && header_count < 100) {
            trim(token);
            headers[header_count] = copy_string(token);
            if (!headers[header_count]) {
                for (int h = 0; h < header_count; h++) free(headers[h]);
                fclose(fp);
                return -1;
            }
            header_count++;
            token = strtok(NULL, ",\r\n");
        }
    }

    for (i = 0; i < FEATURE_COUNT; i++) feature_col[i] = -1;

    for (i = 0; i < header_count; i++) {
        int j;
        for (j = 0; j < FEATURE_COUNT; j++) {
            if (strcmp(headers[i], feature_names[j]) == 0)
                feature_col[j] = i;
        }
        if (strcmp(headers[i], "future_performance_category") == 0)
            label_col = i;
    }

    for (i = 0; i < FEATURE_COUNT; i++) {
        if (feature_col[i] < 0) {
            printf("Error: CSV is missing required column '%s'.\n",
                   feature_names[i]);
            for (int h = 0; h < header_count; h++) free(headers[h]);
            fclose(fp);
            return -1;
        }
    }
    if (label_col < 0) {
        printf("Error: CSV is missing required column 'future_performance_category'.\n");
        for (int h = 0; h < header_count; h++) free(headers[h]);
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) && count < capacity) {
        char *values[100];
        int value_count = 0;
        char *token;
        Student s;
        int valid = 1;

        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;

        token = strtok(line, ",\r\n");
        while (token && value_count < 100) {
            trim(token);
            values[value_count++] = token;
            token = strtok(NULL, ",\r\n");
        }

        if (value_count < header_count) continue;

        for (i = 0; i < FEATURE_COUNT; i++) {
            char *end;
            double v = strtod(values[feature_col[i]], &end);
            if (end == values[feature_col[i]] || *end != '\0') {
                valid = 0;
                break;
            }
            s.x[i] = v;
        }

        s.label = class_from_name(values[label_col]);
        if (s.label < 0) valid = 0;

        if (valid) students[count++] = s;
    }

    for (i = 0; i < header_count; i++) free(headers[i]);
    fclose(fp);

    if (count == 0) {
        printf("Error: no valid student records found.\n");
        return -1;
    }
    return count;
}
