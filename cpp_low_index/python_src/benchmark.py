import os
import sys
import time
import cpp_low_index
from cpp_low_index import permutation_reps
from cpp_low_index import benchmark_util

use_low_level = False
low_level_multi_threaded = False

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
            short_relators = ex['short relators'],
            long_relators = ex['long relators'],
            max_degree = ex['index']))

def run_example_low_level(ex):
    short_relators = cpp_low_index._low_index.spin(
        [ cpp_low_index._low_index.parse_word(
            rank = ex['rank'], word = relator)
          for relator in ex['short relators'] ],
        max_degree = ex['index'])
    long_relators = [
        cpp_low_index._low_index.parse_word(
            rank = ex['rank'], word = relator)
          for relator in ex['long relators'] ]
    if low_level_multi_threaded:
        tree = cpp_low_index._low_index.SimsTreeMultiThreaded(
            rank = ex['rank'],
            max_degree = ex['index'],
            short_relators = short_relators,
            long_relators = long_relators,
            thread_num = cpp_low_index._low_index.hardware_concurrency())
    else:
        tree = cpp_low_index._low_index.SimsTree(
            rank = ex['rank'],
            max_degree = ex['index'],
            short_relators = short_relators,
            long_relators = long_relators)
        
    return len(tree.list())

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
