import cpp_low_index

x = cpp_low_index.permutation_reps(4, [], 5, thread_num = 1)

while x == cpp_low_index.permutation_reps(4, [], 5, thread_num = 10):
    print("Success")
