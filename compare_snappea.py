import time
import snappy
import low_index

def covers_low_index(self, degree, num_short_relators=0, num_threads=0):
    """
    Compute all covers using low_index.  Copied from
    triangulations.pyx so we can set the number of threads.
    """
    G = self.fundamental_group()

    relators = sorted(G.relators(as_int_list=True), key=len)

    
    #if G.num_relators() > self.num_cusps():
    #    num_short_relators = G.num_relators() - self.num_cusps()
    #else:
    #    num_short_relators = G.num_relators()

    short_relators = relators[:num_short_relators]
    long_relators = relators[num_short_relators:]

    return [self.cover(H)
            for H in low_index.permutation_reps(
                    G.num_generators(),
                    short_relators, long_relators,
                    degree, num_threads=num_threads)
            if len(H[0]) == degree]


def compare_methods(manifold, num_short_relators=0, num_threads=4):
    """
    Compare running time of low_index versus the snappea kernel.  For
    any fixed manifold, low_index will always win in the end, so
    compute out until we get to that point.
    """
    M = manifold
    G = M.fundamental_group()
    print(M)
    for degree in range(2, 10):
        start = time.time()
        covers = M.covers(degree, method='snappea')
        snappea_time = time.time() - start

        start = time.time()
        covers = covers_low_index(M, degree, num_short_relators, num_threads)
        low_index_time = time.time() - start

        a = min(snappea_time, low_index_time)
        b = max(snappea_time, low_index_time)
        
        print(degree, a, b, b/a, snappea_time < low_index_time)
        if (snappea_time > low_index_time and degree > 3 and b > 100):
            break


        
                          
