import low_index

x = low_index.permutation_reps(4, [], [], 5, num_threads = 1)

while x == low_index.permutation_reps(4, [], [], 5, num_threads = 10):
    print("Success")
