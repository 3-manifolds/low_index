import os
import sys
import platform
import time
from low_index import *
examples = [
    {
        'group' : 'Fundamental group of K11n34',
        'rank': 3,
        'relators': [
            "aaBcbbcAc",
            "aacAbCBBaCAAbbcBc"
            ],
        'index': 7,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of K11n34',
        'rank': 3,
        'relators': [
            "aaBcbbcAc",
            "aacAbCBBaCAAbbcBc"
            ],
        'index': 8,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of K15n12345',
        'rank': 3,
        'relators': [
            "aBcACAcb",
            "aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb",
            ],
        'index': 7,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of K15n12345',
        'rank': 3,
        'relators': [
            "aBcACAcb",
            "aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb",
            ],
        'index': 8,
        'num_long': 1,
    },
    {
        'group' : 'Fundamental group of o9_15405',
        'rank': 2,
        'relators': [
            "aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"
            ],
        'index': 9,
        'num_long': 0,
    },
    {
        'group' : 'Fundamental group of o9_15405',
        'rank': 2,
        'relators': [
            "aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"
            ],
        'index': 9,
        'num_long': 1,
    },
    {
        'group' : 'Modular Group',
        'rank': 2,
        'relators': [
            "aa",
            "bbb",
            ],
        'index': 25,
        'num_long': 0,
    },
    {
        'group' : 'Symmetric Group S7',
        'rank' : 2,
        'relators': [
            "aaaaaaa",
            "bb",
            "abababababab",
            "AbabAbabAbab",
            "AAbaabAAbaab",
            "AAAbaaabAAAbaaab"],
        'index': 35,
        'num_long': 0,
    }
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

def run(ex):
    print('%s; index = %d.'%(ex['group'], ex['index']))
    t = SimsTree(ex['rank'], ex['index'], ex['relators'], num_long_relators=ex['num_long'])
    start = time.time()
    x = t.list()
    elapsed = time.time() - start
    print('%d subgroups'%len(x))
    print('%.3fs'%elapsed)
    sys.stdout.flush()

if __name__ == '__main__':
    print(cpu_info(), 'with', os.cpu_count(), 'cores')
    for example in examples:
        run(example)
