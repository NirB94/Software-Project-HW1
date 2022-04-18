'''Assignment 1 - Software Project
In this assignment we were asked to implement k-means algorithm in python and in C.
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
    obs = read_file(input_file_path)
    try:
        validate_input(K, max_iter, len(obs))
    except (AssertionError):
        print("Invalid Input!")
        return
    #try:
    clusters = calculate_kmeans(K, max_iter, obs, 0.001)
    #except:
     #   print("An Error Has Occurred")
      #  return
    try:
        write_file(output_file_path, clusters)
    except:
        print("An Error Has Occurred")
        return

def receive_input():
    assert len(sys.argv) in (4, 5) # Check validity of input
    K = int(sys.argv[1])
    max_iter = 200
    i = 1
    if len(sys.argv) == 5: # Means max_iter included in input.
        i += 1
        max_iter = int(sys.argv[2])
    input_file_path = sys.argv[i + 1] # If max_len is not a part of input, this will be second argument.
    output_file_path = sys.argv[i + 2] # Same as above, but 3rd argument.
    return K, max_iter, input_file_path, output_file_path


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


def validate_input(K, max_iter, N):
    assert type(K) == int and type(max_iter) == int \
         and 1<K<N and max_iter > 0


def calculate_kmeans(K, max_iter, obs, epsilon):
    clusters = [[obs[i][0], set()] for i in range(K)] # Initialize clusters: observation and members of cluster
    converged = False
    while (not converged) or max_iter > 0:
        for x in obs: # Check each observation
            minimal_index = 0
            minimal_sum = calculate_norm(x[0], clusters[0][0])
            for i in range(1, K): # Find the cluster that brings norm to minimum.
                curr_sum = calculate_norm(x[0] ,clusters[i][0])
                if curr_sum < minimal_sum:
                    minimal_sum = curr_sum
                    minimal_index = i
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
            if calculate_norm(prev_value, clusters[i][0]) >= epsilon:
                converged = False
        max_iter -= 1
    return clusters


def calculate_norm(x, y): # x and y are vectors of same length
    s = 0
    for i in range(len(x)):
        s += (x[i] - y[i]) ** 2
    return s


def write_file(file_path, clusters):
    f = open(file_path, "w")
    for cluster in clusters:
        st = ",".join(["%.4f" % elem for elem in cluster[0]]) + "\n"
        f.writelines(st)
    f.close()

main()