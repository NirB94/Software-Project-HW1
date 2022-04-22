/* 
Assignment 1 - Software Project
In this assignment we were asked to implement k-means algorithm in python and in C.
Provided here is the C implementation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* 
The function checks if the given string represents a positive integer.
The function uses the builtin function isdigit from the ctype.h library.
*/
int is_natural(char st[]){
    int i;
    for (i = 0; st[i] != '\0'; i++){
        if (isdigit(st[i]) == 0){
            return 1;
        }
    }
    return 0;
}

/*
The function check if the input is of the right length.
Then, the function checks whether K and max_iter (if provided) are valid natural numbers.
*/
int first_input_validation(int length_of_input, char *input[])
{
    if (length_of_input != 4 && length_of_input != 5){
        printf("Invalid Input!");
        return 1;
    }
    if (is_natural(input[1]) == 1 || (length_of_input == 5 && is_natural(input[2]) == 1)) {
        printf("Invalid Input!");
        return 1;
    }
    return 0;
}

/*
The function retrieves the dimension of the input file.
The function inputs said dimensions (# of rows and columns) into the dims array.
*/
int find_dimensions(char const *filename, int *dims){
    FILE *f = NULL;
    char c;

    f = fopen(filename, "r");
    if (f == NULL) {
        fclose(f);
        printf("An Error Has Occurred");
        return 1;
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
    return 0;
}

/*
The function reads the input file.
The function returns a double matrix of size rows * columns whose elements are that of the input file.
*/
double** read_file(char const *filename, int rows, int columns) 
{
    FILE *f = NULL;
    char c;
    int i, j;
    double** obs;

    f = fopen(filename, "r");
    if (f == NULL) {
        fclose(f);
        printf("An Error Has Occurred");
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

/*
The function checks if K and max_iter are of valid values.
*/
int second_input_validation(int K, int N, int max_iter)
{
    if(!(1<K && K<N) || max_iter <= 0) {
        printf("Invalid Input!");
        return 1;
    }
    return 0;
}

/*
The function returns the square of the euclidean distance between the two double arrays.
The function assumes the dimension of the arrays is columns.
*/
double euclid_dist_sq(double* x, double* y, int columns){
    double dist;
    int j;

    dist = 0;
    for (j = 0; j < columns; j++){
        dist += (x[j]-y[j]) * (x[j]-y[j]);
    }
    return dist;
}

/*
The function finds the index of the closest centroid to the array x.
The distance is measured using the euclidean distance.
The function assumes the dimension of the centroids and of x is columns.
The function uses the function euclid_dist_sq.
*/
int find_closest(double** centroids, double* x, int K, int columns){
    double minimal_distance, curr_distance;
    int minimal_index, i;
    
    curr_distance = minimal_distance = euclid_dist_sq(centroids[0], x, columns);
    minimal_index = 0;
    for (i = 1; i < K; i++){
         curr_distance = euclid_dist_sq(centroids[i], x, columns);
         if (minimal_distance > curr_distance){
             minimal_distance = curr_distance;
             minimal_index = i;
         }
    }
    return minimal_index;
}

/*
The function calculates the K cluster centroids produced by the K-means algorithm on the input file.
The function first initializes the centroids to be the first K observations.
The function then iterates performs the following:
- Adding each observation's elements to the sums of its closest cluster, and incrementing
  the updated size of the cluster. 
  Figuring out which is the closet cluster is done using the function find_closest.
- Calculating each cluster's new centroid as the average of the cluster's updated observations.
- Calculating each cluster's deviation between its old centroid and its new one. The deviation is calculated
using the function euclid_dist_sq
The function stops when either max_iter iterations have happend, or when the deviation of any cluster
is less than epsilon squared (the distance itself is less than epsilon).
*/
double** calculate_kmeans(double** obs, int rows, int columns, int K, int max_iter, double epsilon){
    double** old_centroids;
    double** new_centroids;
    int* cluster_counts;
    int i, j, curr_index, converged;
    
    old_centroids = calloc(K, sizeof(double*));
    new_centroids = calloc(K, sizeof(double*));
    cluster_counts = calloc(K, sizeof(int));
    for (i = 0; i < K; i++){
        old_centroids[i] = calloc(columns, sizeof(double));
        new_centroids[i] = calloc(columns, sizeof(double));
        for (j = 0; j < columns; j++){
            old_centroids[i][j] = obs[i][j];
        }
    }
    converged = 0;
    while (converged == 0 && max_iter > 0)
    {
        for (i = 0; i < rows; i++){
            curr_index = find_closest(old_centroids, obs[i], K, columns);
            cluster_counts[curr_index] += 1;
            for (j = 0; j < columns; j++){
                new_centroids[curr_index][j] += obs[i][j];
            }
        }
        converged = 1;
        for (i = 0; i < K; i++){
            if (cluster_counts[i] == 0){
                free(old_centroids);
                free(new_centroids);
                free(cluster_counts);
                printf("An Error Has Occurred");
                return NULL;
            }
            for (j = 0; j < columns; j++){
                new_centroids[i][j] = new_centroids[i][j] / cluster_counts[i];
            }
            if (euclid_dist_sq(new_centroids[i], old_centroids[i], columns) >= (epsilon * epsilon)){
                converged = 0;
            }
        }
        max_iter--;
        for (i = 0; i < K; i++){
            cluster_counts[i] = 0;
            for (j = 0; j < columns; j++){
                old_centroids[i][j] = new_centroids[i][j];
                new_centroids[i][j] = 0;
            }
        }
    }
    free(new_centroids);
    free(cluster_counts);
    return old_centroids;
}

/*
The function writes the K centroids to the output file.
The function assumes that exactly K centroids exist, and that the dimension of the centroids is columns.
*/
int write_file(double** centroids, char const *filename, int columns, int K){
    FILE *f = NULL;
    int i, j;
    
    f = fopen(filename, "w");
    if (f == NULL){
        fclose(f);
        printf("An Error Has Occurred");
        return 1;
    }
    for (i = 0; i < K; i++){
        for (j = 0; j < columns-1; j++){
            fprintf(f, "%.4f,", centroids[i][j]);
        }
        fprintf(f, "%.4f\n", centroids[i][columns-1]);
    }
    fclose(f);
    return 0;
}

int main(int argc, char *argv[])
{
    double** obs;
    double** centroids;
    char* input_file_path;
    char* output_file_path;
    int dims[2];
    int K, max_iter, i;

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
    output_file_path = argv[i+2];
    if (find_dimensions(input_file_path, dims) == 1){
        return 1;
    }
    obs = read_file(input_file_path, dims[0], dims[1]);
    if (obs == NULL)
    {
        return 1;
    }
    if (second_input_validation(K, dims[0], max_iter) == 1)
    {
        free(obs);
        return 1;
    }
    centroids = calculate_kmeans(obs, dims[0], dims[1], K, max_iter, 0.001);
    if (centroids == NULL){
        return 1;
    }
    free(obs);
    if (write_file(centroids, output_file_path, dims[1], K) == 1){
        free(centroids);
        return 1;
    }
    free(centroids);
    return 0;
}