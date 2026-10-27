#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 1000
#define FEATURE_COUNT 15
#define MAX_LINE 4096
#define CLASS_COUNT 4

typedef struct {
    double x[FEATURE_COUNT];
    int label;
} Student;

extern const char *feature_names[FEATURE_COUNT];
extern const char *class_names[CLASS_COUNT];

int load_csv(const char *filename, Student students[], int capacity);
int class_from_name(const char *name);
int predict_knn(const Student train[], int n, const double x[], int k);
void train_tree(const Student train[], int n);
int predict_tree(const double x[]);
double evaluate_knn(const Student data[], const int indices[], int n,
                    const Student train[], int train_n, int k);
double evaluate_tree(const Student data[], const int indices[], int n);

#endif