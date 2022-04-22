'''Assignment 1 - Software Project
In this assignment we were asked to implement k-means algorithm in python and in C.
Provided here is the python implementation.
'''

import sys

def main():
    # Call all functions below
    # if AssertionError is raised - handle it!
    try:
        K, max_iter, input_file_path, output_file_path = receive_input()
    except (AssertionError):
        print("Invalid Input!")
        return
    try:
        obs = read_file(input_file_path)
    except:
        print("An Error Has Occurred")
        return
    try:
        validate_input(K, max_iter, len(obs))
    except (AssertionError):
        print("Invalid Input!")
        return
    try:
        clusters = calculate_kmeans(K, max_iter, obs, 0.001)
    except:
        print("An Error Has Occurred")
        return
    try:
        write_file(output_file_path, clusters)
    except:
        print("An Error Has Occurred")
        return

'''
The function check if the input is of the right length.
Then, the function checks whether K and max_iter (if provided) are valid integers.
'''
def receive_input():
    assert len(sys.argv) in (4, 5) # Check validity of input
    try:
        K = int(sys.argv[1])
    except:
        assert 1 == 0
    max_iter = 200
    i = 1
    if len(sys.argv) == 5: # Means max_iter included in input.
        i += 1
        try:
            max_iter = int(sys.argv[2])
        except:
            assert 2 == 1
    input_file_path = sys.argv[i + 1] # If max_len is not a part of input, this will be second argument.
    output_file_path = sys.argv[i + 2] # Same as above, but 3rd argument.
    return K, max_iter, input_file_path, output_file_path

'''
The function reads the input file.
The function returns a float matrix whose elements are that of the input file.
'''
def read_file(file_path):
    f = open(file_path, "r")
    line = f.readline() # Read first line.
    obs = []
    while line.strip() != "": # Until empty line.
        x = [tuple(float(elem) for elem in line.split(","))] # wrap observation in a list. List meant to hold observation and cluster.
        obs.append(x)
        line = f.readline()
    f.close()
    return obs

'''
The function checks if K and max_iter are of valid values.
'''
def validate_input(K, max_iter, N):
    assert 1<K<N and max_iter > 0

'''
The function calculates the K cluster centroids produced by the K-means algorithm on the input file.
The function first initializes the centroids to be the first K observations.
The function then iterates performs the following:
- Adding each observation to its closest cluster, and removing it from its old one. 
  Figuring out which is the closet cluster is done using the function find_closest.
- Calculating each cluster's new centroid as the average of the cluster's updated observations.
- Calculating each cluster's deviation between its old centroid and its new one. The deviation is calculated
using the function square_euclidean_distance
The function stops when either max_iter iterations have happend, or when the deviation of any cluster
is less than epsilon squared (the distance itself is less than epsilon).
'''
def calculate_kmeans(K, max_iter, obs, epsilon):
    clusters = [[obs[i][0], set()] for i in range(K)] # Initialize clusters: observation and members of cluster
    converged = False
    while (not converged) or max_iter > 0:
        for x in obs: # Check each observation
            minimal_index = find_closest(x, clusters)
            if len(x) != 1: # x has been inserted into a cluster before.
                clusters[x[1]][1].remove(x[0]) # Remove x from the same cluster.
                x[1] = minimal_index
            else:
                x.append(minimal_index)
            clusters[minimal_index][1].add(x[0]) # Now each observation points to its cluster, and that cluster contains the observation.
        converged = True 
        for i in range(K): # Update centroids and check if converged.
            prev_value = clusters[i][0]
            new_value = []
            for j in range(len(clusters[i][0])):
                new_value.append(sum((x[j] for x in clusters[i][1])) / len(clusters[i][1])) # Summation of members of cluster divided by number of members.
            clusters[i][0] = tuple(new_value)
            if square_euclidean_distance(prev_value, clusters[i][0]) >= epsilon ** 2:
                converged = False
        max_iter -= 1
    return clusters

'''
The function returns the square of the euclidean distance between the two float lists.
The function assumes the dimension of the lists is columns.
'''
def square_euclidean_distance(x, y): # x and y are vectors of same length
    s = 0
    for i in range(len(x)):
        s += (x[i] - y[i]) ** 2
    return s

'''
The function finds the index of the closest centroid to x.
The distance is measured using the euclidean distance.
The function assumes the dimension of the centroids and of x is the same.
The function uses the function square_euclidean_distance.
'''
def find_closest(x, clusters):
    minimal_index = 0
    minimal_distance = square_euclidean_distance(x[0], clusters[0][0])
    for i in range(1, len(clusters)): # Find the cluster that brings distance to minimum.
        curr_distance = square_euclidean_distance(x[0] ,clusters[i][0])
        if curr_distance < minimal_distance:
            minimal_distance = curr_distance
            minimal_index = i
    return minimal_index

'''
The function writes the K centroids to the output file.
The function assumes that exactly K centroids exist, 
and that the dimension of the centroids is the same.
'''
def write_file(file_path, clusters):
    f = open(file_path, "w")
    for cluster in clusters:
        st = ",".join(["%.4f" % elem for elem in cluster[0]]) + "\n"
        f.writelines(st)
    f.close()

if __name__ == "__main__":
    main()