
#include <stdio.h>
#include "preprocessing.h"

int main(void)
{
    Student data[4] = {0};
    FeatureScaler scaler;
    int i, j;

    /* Create four sample student records */
    for (i = 0; i < 4; i++)
    {
        data[i].label = i;

        for (j = 0; j < FEATURE_COUNT; j++)
        {
            if (j == 0 || j == 1)
                data[i].x[j] = 2.0 + i * 0.5;

            else if (j >= 2 && j <= 8)
                data[i].x[j] = 50.0 + i * 10.0;

            else if (j == 9)
                data[i].x[j] = 2.0 + i;

            else if (j == 10 || j == 11 ||
                     j == 13 || j == 14)
                data[i].x[j] = 60.0 + i * 10.0;

            else if (j == 12)
                data[i].x[j] = i;
        }
    }

    /* Fit scaler using the first three students */
    if (!fit_feature_scaler(data, 3, &scaler))
    {
        printf("Scaler training failed.\n");
        return 1;
    }

    printf("Scaler fitted successfully.\n");

    /* Normalize all four students */
    if (!transform_dataset(data, 4, &scaler))
    {
        printf("Normalization failed.\n");
        return 1;
    }

    printf("\nNormalized CGPA values:\n");

    for (i = 0; i < 4; i++)
    {
        printf("Student %d: %.2f\n",
               i + 1, data[i].x[0]);
    }

    printf("\nPreprocessing completed successfully.\n");

    return 0;
}
