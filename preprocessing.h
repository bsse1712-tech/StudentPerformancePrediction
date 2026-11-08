
#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include "student.h"

/* Store minimum and maximum values of all features */
typedef struct {
    double min[FEATURE_COUNT];
    double max[FEATURE_COUNT];
    int fitted;
} FeatureScaler;

/* Calculate minimum and maximum from training data */
int fit_feature_scaler(const Student training_data[],
                       int train_count,
                       FeatureScaler *scaler);

/* Normalize one student's features */
int transform_features(double features[],
                       const FeatureScaler *scaler);

/* Normalize an entire dataset */
int transform_dataset(Student data[],
                      int count,
                      const FeatureScaler *scaler);

#endif
