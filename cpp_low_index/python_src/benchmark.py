import os
import sys
import time
import subprocess
from tempfile import NamedTemporaryFile
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
        'gap skip': True,
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
            num_threads = cpp_low_index._low_index.hardware_concurrency())
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

def translate_to_gap(ex, output):
    output.write('info := "%s; index=%d";\n'%(ex['group'], ex['index']))
    all_relators = ex['short relators'] + ex['long relators']
    gap_relators = [benchmark_util.gap_relator(r) for r in all_relators]
    letters = 'abcdefghijklmnopqrstuvwxyz'
    generators = letters[:ex['rank']]
    output.write('F := FreeGroup(')
    output.write(', '.join(['"%s"'%g for g in generators]))
    output.write(');\n')
    for n, gen in enumerate(generators):
        output.write('%s := F.%d;\n'%(gen, n + 1))
    output.write('G := F / [\n')
    for relator in gap_relators:
        output.write('%s,\n'%relator)
    output.write('];\n')
    output.write("""
PrintFormatted("{}\\n", info);
start := NanosecondsSinceEpoch();
ans := Length(LowIndexSubgroupsFpGroup(G,%d));
elapsed := Round(Float(NanosecondsSinceEpoch() - start) / 10000000.0)/100;
PrintFormatted("{} subgroups\\n", ans);
PrintFormatted("{} secs\\n", ViewString(elapsed));
"""%ex['index'])

if __name__ == '__main__':
    print(benchmark_util.cpu_info(),
          'with',
          os.cpu_count(),
          'cores (reported by python)/',
          cpp_low_index._low_index.hardware_concurrency(),
          'cores (reported by C++)', file=sys.stderr)
    if '-gap' in sys.argv:
        with open('/tmp/benchmark.gap', 'w') as gap_script:
            for example in examples:
                if 'gap skip' not in example:
                    translate_to_gap(example, gap_script)
            gap_script.write("QUIT;\n")
        subprocess.run(['sage', '-gap', '/tmp/benchmark.gap'])
        os.unlink('/tmp/benchmark.gap')
    else:
        for example in examples:
            run(example)

