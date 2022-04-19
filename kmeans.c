/* Assignment 1 - Software Project
In this assignment we were asked to implement k-means algorithm in python and in C.
Provided here the C implementation.*/

#include <stdio.h>
#include <stdlib.h>

int first_input_validation(int length_of_input, char *input[])
{
    if (length_of_input != 4 && length_of_input != 5){
        printf("Invalid Input!");
        return 1;
    }
    if (atoi(input[1]) == 0 || (length_of_input == 5 && atoi(input[2]) == 0)) {
        printf("Invalid Input!");
        return 1;
    }
    return 0;
}

void find_dimensions(char const *filename, int *dims){
    FILE *f = NULL;
    char c;

    f = fopen(filename, "r");
    if (f == NULL) {
        fclose(f);
        return;
    }
    dims[0] = 0;
    dims[1] = 1;
    while ((c = fgetc(f)) != EOF) 
    {
        if (c == '\n')
        {
            dims[0]++;
        }
        else if (c == ',')
        {
            if (dims[0] == 0)
            {
                dims[1]++;
            }
        }
    }
    fclose(f);
}

double** read_file(char const *filename, int rows, int columns) 
{
    FILE *f = NULL;
    char c;
    int i, j;
    double** obs;

    f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }
    obs = calloc(rows, sizeof(double*));
    for (i = 0; i < rows; i++)
    {
        obs[i] = calloc(columns, sizeof(double));
        for (j = 0; j < columns; j++)
        {
            fscanf(f, "%lf%c", &obs[i][j], &c);
        }
    }
    fclose(f);
    return obs;
}

int second_input_validation(int K, int N, int max_iter)
{
    if(!(1<K && K<N) || max_iter <= 0) {
        printf("Invalid Input!");
        return 1;
    }
    return 0;
}

double** calculate_kmeans(double** obs, int rows, int columns, int k, int max_iter){
    double** old_centroids;
    double** new_centroids;
    int* cluster_count;
    int i, j, curr_index;
    
    old_centroids = calloc(k, sizeof(double*));
    new_centroids = calloc(k, sizeof(double*));
    cluster_count = calloc(k, sizeof(int));
    for (i = 0; i < k ; i++){
        old_centroids[i] = calloc(columns, sizeof(double));
        new_centroids[i] = calloc(columns, sizeof(double));
        for (j = 0; j < columns; j++){
            old_centroids[i][j] = obs[i][j];
        }
    }
    for (i = 0; i < rows; i++){
        curr_index = find_closest(old_centroids, obs[i], k, columns);
        cluster_count[curr_index] += 1;
        for (j = 0; j < columns; j++){
            new_centroids[i][j] += obs[i][j];
        }
    }

}

int find_closest(double** centroids, double* x, int k, int columns){
    double minimal_distance, curr_distance;
    int minimal_index, i;
    
    curr_distance = minimal_distance = find_norm(centroids[0], x, columns);
    minimal_index = 0;
    for (i = 1; i < k; i++){
         curr_distance = find_norm(centroids[i], x, columns);
         if (minimal_distance > curr_distance){
             minimal_distance = curr_distance;
             minimal_index = i;
         }
    }
    return minimal_index;
}

double find_norm(double* x, double* y, int columns){
    double norm;
    int j;

    norm = 0;
    for (j = 0; j < columns; j++){
        norm += (x[j]-y[j]) * (x[j]-y[j]);
    }
    return norm;
}


int main(int argc, char *argv[])
{
    double** obs;
    int K, max_iter, i;
    char* input_file_path;
    int dims[2];
    /*char* output_file_path;*/

    if(first_input_validation(argc, argv) == 1){
        return 1;
    }
    K = atoi(argv[1]);
    max_iter = 200;
    i = 1;
    if (argc == 5) {
        i++;
        max_iter = atoi(argv[i]);
    }
    input_file_path = argv[i+1];
    /*output_file_path = argv[i+2];*/
    find_dimensions(input_file_path, dims);
    obs = read_file(input_file_path, dims[0], dims[1]);
    if (obs == NULL) /*Condition may be irrelevant, consider deleting.*/
    {
        return 1;
    }
    if (second_input_validation(K, dims[0], max_iter) == 1)
    {
        return 1;
    }
    return 0;
}