
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "preprocessing.h"

/* Calculate minimum and maximum values */
int fit_feature_scaler(const Student training_data[],
                       int train_count,
                       FeatureScaler *scaler)
{
    int i, j;

    if (training_data == NULL || scaler == NULL ||
        train_count <= 0)
        return 0;

    scaler->fitted = 0;

    /* Initialize minimum and maximum values */
    for (j = 0; j < FEATURE_COUNT; j++)
    {
        scaler->min[j] = DBL_MAX;
        scaler->max[j] = -DBL_MAX;
    }

    /* Find minimum and maximum for every feature */
    for (i = 0; i < train_count; i++)
    {
        for (j = 0; j < FEATURE_COUNT; j++)
        {
            double value = training_data[i].x[j];

            if (!isfinite(value))
                return 0;

            if (value < scaler->min[j])
                scaler->min[j] = value;

            if (value > scaler->max[j])
                scaler->max[j] = value;
        }
    }

    scaler->fitted = 1;

    return 1;
}

/* Normalize one student's feature array */
int transform_features(double features[],
                       const FeatureScaler *scaler)
{
    int i;

    if (features == NULL || scaler == NULL ||
        !scaler->fitted)
        return 0;

    /* Check values before modifying them */
    for (i = 0; i < FEATURE_COUNT; i++)
    {
        if (!isfinite(features[i]))
            return 0;
    }

    for (i = 0; i < FEATURE_COUNT; i++)
    {
        double range = scaler->max[i] - scaler->min[i];

        if (range == 0.0)
        {
            features[i] = 0.0;
        }
        else
        {
            features[i] =
                (features[i] - scaler->min[i]) / range;
        }
    }

    return 1;
}

/* Normalize all students in a dataset */
int transform_dataset(Student data[],
                      int count,
                      const FeatureScaler *scaler)
{
    int i, j;

    if (data == NULL || scaler == NULL ||
        !scaler->fitted || count <= 0)
        return 0;

    /* Check all records before changing the dataset */
    for (i = 0; i < count; i++)
    {
        for (j = 0; j < FEATURE_COUNT; j++)
        {
            if (!isfinite(data[i].x[j]))
                return 0;
        }
    }

    /* Apply normalization to every student */
    for (i = 0; i < count; i++)
    {
        if (!transform_features(data[i].x, scaler))
            return 0;
    }

    return 1;
}
