import os
import sys
import time
import cpp_low_index
from cpp_low_index import permutation_reps
from cpp_low_index import benchmark_util
examples = [
    {
        'group' : 'Fundamental group of K11n34',
        'rank': 3,
        'short relators': ["aaBcbbcAc"],
        'long relators': ["aacAbCBBaCAAbbcBc"],
        'index': 7,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of K11n34',
        'rank': 3,
        'short relators': ["aaBcbbcAc"],
        'long relators':  ["aacAbCBBaCAAbbcBc"],
        'index': 8,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of K15n12345',
        'rank': 3,
        'short relators': ["aBcACAcb"],
        'long relators': ["aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb"],
        'index': 7,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of K15n12345',
        'rank': 3,
        'short relators': ["aBcACAcb"],
        'long relators':["aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb"],
        'index': 8,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of o9_15405',
        'rank': 2,
        'short relators': ["aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"],
        'long relators': [],
        'index': 9,
        'num_long': 0,
    },
    {
        'group' : 'Fundamental group of o9_15405',
        'rank': 2,
        'short relators': [],
        'long relators': ["aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"],
        'index': 9,
        'num_long': 1,
    },
    {
        'group' : 'Modular Group',
        'rank': 2,
        'short relators': ["aa", "bbb"],
        'long relators': [],
        'index': 25,
        'num_long': 0,
    },
    {
        'group' : 'Symmetric Group S7',
        'rank' : 2,
        'short relators': [
            "aaaaaaa",
            "bb",
            "abababababab",
            "AbabAbabAbab",
            "AAbaabAAbaab",
            "AAAbaaabAAAbaaab"],
        'long relators': [],
        'index': 35,
        'num_long': 0,
    }
]

def run_example(ex):
    return len(
        permutation_reps(
            rank = ex['rank'],
            relators = ex['short relators'] + ex['long relators'],
            max_degree = ex['index'],
            num_long_relators = ex['num_long'],
            bloom_size = 2000))

def run_example_low_level(ex):
    short_relators, long_relators = (
        cpp_low_index._low_index.compute_short_and_long_relators(
            rank = ex['rank'],
            relators = [
                cpp_low_index._low_index.parse_word(ex['rank'], w)
                for w in ex['short relators'] + ex['long relators']],
            max_degree = ex['index'],
            num_long_relators = ex['num_long']))
    tree = cpp_low_index._low_index.SimsTree(
        rank = ex['rank'],
        max_degree = ex['index'],
        short_relators = short_relators,
        long_relators = long_relators)
    # thread_num = 0 makes SimsTree determine the number of
    # threads by using std::thread::hardware_concurrency()
    return len(tree.list(bloom_size = 2000, thread_num = 0))

use_low_level = False

def run(ex):
    print('%s; index = %d.'%(ex['group'], ex['index']))
    start = time.time()
    if use_low_level:
        n = run_example_low_level(ex)
    else:
        n = run_example(ex)
    elapsed = time.time() - start
    print('%d subgroups' % n)
    print('%.3fs'%elapsed)
    sys.stdout.flush()

if __name__ == '__main__':
    print(benchmark_util.cpu_info(),
          'with',
          os.cpu_count(),
          'cores (reported by python)/',
          cpp_low_index._low_index.hardware_concurrency(),
          'cores (reported by C++)')
    for example in examples:
        run(example)
