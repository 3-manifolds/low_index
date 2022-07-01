import os, platform

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

def expand_relator(word):
    syllables = []
    for letter in word:
        if letter.isupper():
            syllables.append('%s^-1'%letter.lower())
        else:
            syllables.append(letter)
    return '*'.join(syllables)
