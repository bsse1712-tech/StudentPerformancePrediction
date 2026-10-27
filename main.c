
    #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "student.h"

static void print_feature_list(void)
{
    int i;
    printf("\nEnter the following %d numeric features in order:\n", FEATURE_COUNT);
    for (i = 0; i < FEATURE_COUNT; i++)
        printf("  %2d. %s\n", i + 1, feature_names[i]);
}

int main(int argc, char *argv[])
{
    Student data[MAX_STUDENTS], train[MAX_STUDENTS];
    int test_indices[MAX_STUDENTS];
    int n, train_n = 0, test_n = 0, i, k = 5;
    double new_x[FEATURE_COUNT];
    char answer;

    if (argc < 2) {
        printf("Usage: %s student_file.csv\n", argv[0]);
        printf("Example: %s students_01.csv\n", argv[0]);
        return 1;
    }

    n = load_csv(argv[1], data, MAX_STUDENTS);
    if (n < 0) return 1;
    if (n < 10) {
        printf("Please use a dataset with at least 10 valid records.\n");
        return 1;
    }

    printf("\nLoaded %d valid student records from %s.\n", n, argv[1]);

    /* Random 80/20 split. Training and testing records do not overlap. */
    {
        int order[MAX_STUDENTS];
        srand((unsigned int)time(NULL));
        for (i = 0; i < n; i++) order[i] = i;
        for (i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = order[i];
            order[i] = order[j];
            order[j] = temp;
        }

        train_n = (n * 8) / 10;
        if (train_n < 2) train_n = 2;
        if (train_n >= n) train_n = n - 1;
        test_n = n - train_n;

        for (i = 0; i < train_n; i++) {
            train[i] = data[order[i]];
        }
        for (i = 0; i < test_n; i++) test_indices[i] = order[train_n + i];
    }

    printf("\nTraining records: %d | Testing records: %d\n", train_n, test_n);
    printf("KNN uses k = %d.\n", k);

    printf("\nTraining Decision Tree...\n");
    train_tree(train, train_n);

    printf("\nTest-set results (random split; results can vary between runs):\n");
    printf("KNN accuracy:          %.2f%%\n",
           evaluate_knn(data, test_indices, test_n, train, train_n, k));

    /*
     * Decision-tree evaluation uses the same held-out records.
     * The tree was trained only on the training records above.
     */
    printf("Decision Tree accuracy: %.2f%%\n",
           evaluate_tree(data, test_indices, test_n));

    printf("\nWould you like to predict a new student's performance? (y/n): ");
    if (scanf(" %c", &answer) != 1 || (answer != 'y' && answer != 'Y')) {
        printf("Finished.\n");
        return 0;
    }

    print_feature_list();
    for (i = 0; i < FEATURE_COUNT; i++) {
        printf("%s: ", feature_names[i]);
        if (scanf("%lf", &new_x[i]) != 1) {
            printf("Invalid numeric input.\n");
            return 1;
        }
    }

    {
        int knn_prediction = predict_knn(train, train_n, new_x, k);
        int tree_prediction = predict_tree(new_x);

        printf("\nPredicted future performance category:\n");
        if (knn_prediction >= 0)
            printf("KNN:          %s\n", class_names[knn_prediction]);
        else
            printf("KNN:          prediction failed\n");

        if (tree_prediction >= 0)
            printf("Decision Tree: %s\n", class_names[tree_prediction]);
        else
            printf("Decision Tree: prediction failed\n");
    }

    printf("\nNote: predictions are estimates based on the provided synthetic dataset,\n");
    printf("not guaranteed academic outcomes. Accuracy depends on dataset quality.\n");
    return 0;
}
