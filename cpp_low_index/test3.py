import cpp_low_index

x = cpp_low_index.permutation_reps(4, [], [], 5, num_threads = 1)

while x == cpp_low_index.permutation_reps(4, [], [], 5, num_threads = 10):
    print("Success")
