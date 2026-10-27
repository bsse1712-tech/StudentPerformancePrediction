# Student Academic Performance Prediction in C

This project reads a student dataset from a CSV file and predicts the
`future_performance_category` using two classification algorithms:

1. K-Nearest Neighbors (KNN)
2. Decision Tree (numeric feature splits using Gini impurity)

It also makes a random 80/20 training/testing split and reports test-set
classification accuracy for both algorithms.

## Required CSV columns

The CSV must contain these exact headers:

- `previous_cgpa`
- `current_cgpa`
- `introduction_to_algorithms_mark`
- `theory_of_computing_mark`
- `computer_networking_mark`
- `linear_algebra_mark`
- `business_communication_mark`
- `computer_networking_lab_mark`
- `algorithm_lab_mark`
- `daily_study_hours`
- `attendance_percentage`
- `assignment_submission_rate`
- `previous_failed_courses`
- `lab_practical_performance`
- `class_participation`
- `future_performance_category`

Other columns may also be present; the program finds the required columns by
header name. The target labels must be exactly one of:

`At Risk`, `Average`, `Good`, `Excellent`

This first version uses numeric features only. It does not currently use
categorical fields such as `residence`, `does_tuition`, `financial_condition`,
or `scholarship`.

## Compile

With GCC:

```bash
gcc -std=c11 -Wall -Wextra main.c dataset.c models.c -o student_predict -lm
```

## Run

```bash
./student_predict students_01.csv
```

Replace `students_01.csv` with the name/path of your CSV file.

On Windows, the compiled program may be named `student_predict.exe`.

## Notes

- Use synthetic or anonymized records only.
- The random train/test split means the accuracy may differ each run.
- The dataset should have enough examples from each category for meaningful
  evaluation.
- This is an educational baseline, not a validated system for real academic
  decisions.