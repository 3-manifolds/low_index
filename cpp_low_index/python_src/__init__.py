import os, platform
from multiprocessing import cpu_count
from . import _low_index
__version__ = '1.2'

def version():
    return __version__

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

def num_threads():
    cpu = cpu_info()
    if cpu.startswith('Intel'):
        return cpu_count() // 2
    else:
        return cpu_count()
    
class SimsTree(_low_index.SimsTree):
    def __init__(self, rank, max_degree, short_relators, long_relators):
        short_relators = [_low_index.parse_word(rank, r) if isinstance(r, str) else r
                              for r in short_relators]
        long_relators = [_low_index.parse_word(rank, r) if isinstance(r, str) else r
                              for r in long_relators]
        _low_index.SimsTree.__init__(self, rank, max_degree, short_relators,
                                         long_relators)

    def covers(self):
        return self.list(3000, num_threads())
