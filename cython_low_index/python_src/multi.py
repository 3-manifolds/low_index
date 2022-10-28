import low_index, pickle, time, sys
import multiprocessing

def subtree_list(data):
    rank, max_degree, rels, strategy, sims_node=data
    subgroups = low_index.SimsTree(rank, max_degree, rels, strategy,
                                  sims_node).list(use_mp=False)
    return [pickle.dumps(s) for s in subgroups]

def main(rank, max_degree, bloom_size, relators):
    if sys.platform != 'win32':
        context = multiprocessing.get_context('fork')
    else:
        context = multiprocessing.get_context('spawn')
    pool = context.Pool(multiprocessing.cpu_count())
    tree = low_index.SimsTree(rank, max_degree, relators, 'spin_short')
    stems = tree.bloom(bloom_size)
    nodes = [n for n in stems if not n.is_complete()]
    subgroups = [pickle.dumps(n) for n in stems if n.is_complete()]
    inputs = [(rank, max_degree, relators, 'spin_short', node)
                  for node in nodes]
    subgroups += sum(pool.map(subtree_list, inputs), [])
    for subgroup in subgroups:
        sys.stdout.buffer.write(len(subgroup).to_bytes(4, 'big') + subgroup)
        sys.stdout.flush()

if __name__ == '__main__':
    rank, max_degree, bloom_size = (int(arg) for arg in sys.argv[1:4])
    relators = sys.argv[4:]
    main(rank, max_degree, bloom_size, relators)
