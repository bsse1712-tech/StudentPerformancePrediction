#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "student.h"

/* ---------- K-Nearest Neighbors (KNN) ---------- */

typedef struct {
    double distance;
    int label;
} Neighbor;

static double distance_between(const double a[], const double b[])
{
    double sum = 0.0;
    int i;
    for (i = 0; i < FEATURE_COUNT; i++) {
        double d = a[i] - b[i];
        sum += d * d;
    }
    return sqrt(sum);
}

int predict_knn(const Student train[], int n, const double x[], int k)
{
    Neighbor *neighbors;
    int votes[CLASS_COUNT] = {0};
    int i, j, best_class = 0;

    if (n <= 0) return -1;
    if (k < 1) k = 1;
    if (k > n) k = n;

    neighbors = (Neighbor *)malloc((size_t)n * sizeof(Neighbor));
    if (!neighbors) return -1;

    for (i = 0; i < n; i++) {
        neighbors[i].distance = distance_between(train[i].x, x);
        neighbors[i].label = train[i].label;
    }

    /* Selection sort is sufficient for the small educational datasets. */
    for (i = 0; i < k; i++) {
        int smallest = i;
        for (j = i + 1; j < n; j++) {
            if (neighbors[j].distance < neighbors[smallest].distance)
                smallest = j;
        }
        if (smallest != i) {
            Neighbor temp = neighbors[i];
            neighbors[i] = neighbors[smallest];
            neighbors[smallest] = temp;
        }
        votes[neighbors[i].label]++;
    }

    for (i = 1; i < CLASS_COUNT; i++) {
        if (votes[i] > votes[best_class]) best_class = i;
    }

    free(neighbors);
    return best_class;
}

/* ---------- Decision Tree classifier (Gini split, numeric features) ---------- */

#define MAX_NODES 4096
#define MAX_DEPTH 4
#define MIN_LEAF 3

typedef struct {
    int is_leaf;
    int prediction;
    int feature;
    double threshold;
    int left;
    int right;
} TreeNode;

static TreeNode nodes[MAX_NODES];
static int node_count;
static const Student *tree_data;
static int tree_n;

static int majority_class(const int indices[], int n)
{
    int counts[CLASS_COUNT] = {0};
    int i, best = 0;
    for (i = 0; i < n; i++) counts[tree_data[indices[i]].label]++;
    for (i = 1; i < CLASS_COUNT; i++)
        if (counts[i] > counts[best]) best = i;
    return best;
}

static double gini_counts(const int counts[], int n)
{
    double g = 1.0;
    int i;
    if (n <= 0) return 0.0;
    for (i = 0; i < CLASS_COUNT; i++) {
        double p = (double)counts[i] / n;
        g -= p * p;
    }
    return g;
}

static int build_node(const int indices[], int n, int depth)
{
    int this_node, i, f, current_majority;
    int parent_counts[CLASS_COUNT] = {0};
    int all_same = 1;
    double best_gini = 1e9, best_threshold = 0.0;
    int best_feature = -1;

    if (node_count >= MAX_NODES - 2 || n <= 0) return -1;

    this_node = node_count++;
    current_majority = majority_class(indices, n);
    nodes[this_node].is_leaf = 1;
    nodes[this_node].prediction = current_majority;
    nodes[this_node].feature = -1;
    nodes[this_node].left = nodes[this_node].right = -1;

    for (i = 0; i < n; i++) {
        parent_counts[tree_data[indices[i]].label]++;
        if (tree_data[indices[i]].label != tree_data[indices[0]].label)
            all_same = 0;
    }
    if (all_same || depth >= MAX_DEPTH || n < 2 * MIN_LEAF) return this_node;

    for (f = 0; f < FEATURE_COUNT; f++) {
        int a, b;
        for (a = 0; a < n; a++) {
            for (b = a + 1; b < n; b++) {
                double va = tree_data[indices[a]].x[f];
                double vb = tree_data[indices[b]].x[f];
                double threshold;
                int left_counts[CLASS_COUNT] = {0};
                int right_counts[CLASS_COUNT] = {0};
                int left_n = 0, right_n = 0, t;
                double weighted;

                if (va == vb) continue;
                threshold = (va + vb) / 2.0;

                for (t = 0; t < n; t++) {
                    const Student *s = &tree_data[indices[t]];
                    if (s->x[f] <= threshold) {
                        left_counts[s->label]++;
                        left_n++;
                    } else {
                        right_counts[s->label]++;
                        right_n++;
                    }
                }
                if (left_n < MIN_LEAF || right_n < MIN_LEAF) continue;

                weighted = ((double)left_n / n) * gini_counts(left_counts, left_n)
                         + ((double)right_n / n) * gini_counts(right_counts, right_n);
                if (weighted < best_gini) {
                    best_gini = weighted;
                    best_feature = f;
                    best_threshold = threshold;
                }
            }
        }
    }

    if (best_feature < 0) return this_node;

    {
        int *left_indices = (int *)malloc((size_t)n * sizeof(int));
        int *right_indices = (int *)malloc((size_t)n * sizeof(int));
        int left_n = 0, right_n = 0, t;
        int left_node, right_node;

        if (!left_indices || !right_indices) {
            free(left_indices);
            free(right_indices);
            return this_node;
        }

        for (t = 0; t < n; t++) {
            int idx = indices[t];
            if (tree_data[idx].x[best_feature] <= best_threshold)
                left_indices[left_n++] = idx;
            else
                right_indices[right_n++] = idx;
        }

        left_node = build_node(left_indices, left_n, depth + 1);
        right_node = build_node(right_indices, right_n, depth + 1);
        free(left_indices);
        free(right_indices);

        if (left_node >= 0 && right_node >= 0) {
            nodes[this_node].is_leaf = 0;
            nodes[this_node].feature = best_feature;
            nodes[this_node].threshold = best_threshold;
            nodes[this_node].left = left_node;
            nodes[this_node].right = right_node;
        }
    }
    return this_node;
}

void train_tree(const Student train[], int n)
{
    int *indices, i;
    node_count = 0;
    tree_data = train;
    tree_n = n;
    if (n <= 0) return;

    indices = (int *)malloc((size_t)n * sizeof(int));
    if (!indices) return;
    for (i = 0; i < n; i++) indices[i] = i;
    build_node(indices, n, 0);
    free(indices);
}

int predict_tree(const double x[])
{
    int node = 0, steps = 0;
    if (tree_n <= 0 || node_count <= 0) return -1;

    while (node >= 0 && node < node_count && !nodes[node].is_leaf &&
           steps++ <= MAX_DEPTH + 1) {
        if (x[nodes[node].feature] <= nodes[node].threshold)
            node = nodes[node].left;
        else
            node = nodes[node].right;
    }
    if (node < 0 || node >= node_count) return -1;
    return nodes[node].prediction;
}

double evaluate_knn(const Student data[], const int indices[], int n,
                    const Student train[], int train_n, int k)
{
    int correct = 0, i;
    if (n <= 0) return 0.0;
    for (i = 0; i < n; i++) {
        int idx = indices[i];
        if (predict_knn(train, train_n, data[idx].x, k) == data[idx].label)
            correct++;
    }
    return 100.0 * correct / n;
}

double evaluate_tree(const Student data[], const int indices[], int n)
{
    int correct = 0, i;
    if (n <= 0) return 0.0;
    for (i = 0; i < n; i++) {
        int idx = indices[i];
        if (predict_tree(data[idx].x) == data[idx].label) correct++;
    }
    return 100.0 * correct / n;
}
