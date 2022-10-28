import os
import sys
import platform
import time
from low_index import *

examples = [
    {
        'group' : 'Fundamental group of K11n34',
        'rank': 3,
        'short relators': ["aaBcbbcAc"],
        'long relators': ["aacAbCBBaCAAbbcBc"],
        'index': 7,
    },
    {
        'group' : 'Fundamental group of K11n34',
        'rank': 3,
        'short relators': ["aaBcbbcAc"],
        'long relators':  ["aacAbCBBaCAAbbcBc"],
        'index': 8,
    },
    {
        'group' : 'Fundamental group of K15n12345',
        'rank': 3,
        'short relators': ["aBcACAcb"],
        'long relators': ["aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb"],
        'index': 7,
    },
    {
        'group' : 'Fundamental group of K15n12345',
        'rank': 3,
        'short relators': ["aBcACAcb"],
        'long relators':["aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb"],
        'index': 8,
    },
    {
        'group' : 'Fundamental group of o9_15405',
        'rank': 2,
        'short relators': ["aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"],
        'long relators': [],
        'index': 9,
    },
    {
        'group' : 'Fundamental group of o9_15405',
        'rank': 2,
        'short relators': [],
        'long relators': ["aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"],
        'index': 9,
    },
    {
        'group': 'K5_13(3,0)',
        'rank': 2,
        'short relators': ['bbb', 'abABaaBAbabABaaBAbabABAb'],
        'long relators': [],
        'index': 15
    },
    {
        'group': 'K15n145097(0,1)',
        'rank': 3,
        'short relators': ['aBCBabaBAbcbAccAbC'],  # Fastest was just one short rel
        'long relators': ['aBAbcbAcbAcAbAcAbAcAbCaBC',
                          'aBCBabABaaaaaaaaaaaaaaaaBAbcbAccbAcbAcAC'],
        'index': 7,
    },
    {
        'group': 'DodecahedralOrientableClosedCensus[3]',
        'rank': 4,
        'short relators': ['abbDDabacdc',
                           'acdAcDc',
                           'aBacddBBcD',
                           'acddCdCbAddB'],
        'long relators': [],
        'index': 6,
    },
    {
        'group': 'DodecahedralOrientableClosedCensus[8]',
        'rank': 4,
        'short relators': ['adADAddAbdbdbadaDD',
                           'adaDDbcccDAddAbdb',
                           'adaDDbccACACdaDDadCCBdAbcBdAbcBD',
                           'acacaDAddADcacaCCBdAbcBdAbcBdAbc'],
        'long relators': [],  # It didn't seem to help to make some rels long.
        'index': 5,
    },
    {
        'group': 'L14a26995(2,0)(2,0)(2,0)',
        'rank': 4,
        'short relators': ['bb',
                           'aa',
                           'cdcd',
                           'aCABacAddcDbABdCDDaCAbac',
                           'aBdcDbABddcDbaBdCDbABacAb',
                           'aCAbacACBDCbcaCAbacACBcdbcaCABacACBc'],
        'long relators': [], # Making rels long *slowed things down*.
        'index': 7,
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
    },
    # This example must go at the end to avoid an infinite loop when
    # using Magma for the benchmark.
    {
        'group' : 'Modular Group',
        'rank': 2,
        'short relators': ["aa", "bbb"],
        'long relators': [],
        'index': 25,
        'gap skip': True,
    },
]

def cpu_info(): 
    if platform.system() == 'Windows': 
        return platform.processor() 
    elif platform.system() == 'Darwin': 
        command = '/usr/sbin/sysctl -n machdep.cpu.brand_string' 
        return os.popen(command).read().strip() 
    elif platform.system() == 'Linux':
        with open('/proc/cpuinfo') as input_file:
            info = input_file.read().split('\n')
            for line in info:
                parts = line.split(':')
                if parts[0].strip() == 'model name':
                    return parts[1].strip()
    return 'Unknown CPU' 

def run(ex, multi=True):
    print('%s; index = %d.'%(ex['group'], ex['index']))
    relators = ex['short relators'] + ex['long relators']
    t = SimsTree(ex['rank'], ex['index'], relators , num_long_relators=len(ex['long relators']))
    start = time.time()
    x = t.list(multi)
    elapsed = time.time() - start
    print('%d subgroups'%len(x))
    print('%.3fs\n'%elapsed)
    sys.stdout.flush()

if __name__ == '__main__':
    print(cpu_info(), 'with', os.cpu_count(), 'cores')
    multi = not '-single' in sys.argv
    if not multi:
        print('Running single-threaded')
    for example in examples:
        run(example, multi)
