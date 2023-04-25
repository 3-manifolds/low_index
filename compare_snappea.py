import time
import snappy
import low_index

def covers_low_index(self, degree, num_threads=0):
    """
    Compute all covers using low_index.  Copied from
    triangulations.pyx so we can set the number of threads.
    """
    G = self.fundamental_group()

    relators = sorted(G.relators(as_int_list=True), key=len)

    short_relators, long_relators = [], relators
    strategy = ''
    
    if relators:
        if len(relators[0]) <= 2*degree:
            short_relators = [rel for rel in relators if len(rel) <= 2*degree]
            long_relators = relators[len(short_relators):]
            strategy = 'spin_short'
        elif len(relators[0]) <= 3*degree:
            short_relators = [rel for rel in relators if len(rel) <= 3*degree]
            long_relators = relators[len(short_relators):]
            
    print(len(short_relators), strategy)
    return [self.cover(H)
            for H in low_index.permutation_reps(
                    G.num_generators(),
                    short_relators, long_relators,
                    degree, strategy='', num_threads=num_threads)
            if len(H[0]) == degree]


def compare_methods(manifold, num_threads=4):
    """
    Compare running time of low_index versus the snappea kernel.  For
    any fixed manifold, low_index will always win in the end, so
    compute out until we get to that point.
    """
    M = manifold
    G = M.fundamental_group()
    print(M)
    for degree in range(2, 8):
        start = time.time()
        covers = M.covers(degree, method='snappea')
        snappea_time = time.time() - start

        start = time.time()
        covers = covers_low_index(M, degree, num_threads)
        low_index_time = time.time() - start
        
        a = min(snappea_time, low_index_time)
        b = max(snappea_time, low_index_time)
        
        print(degree, a, b, b/a, snappea_time < low_index_time)
        if (snappea_time > low_index_time and degree > 3 and b > 100):
            break


        
                          
