try:
    from ._low_index import *
except:
    # The import can fail when loaded by setup.py
    pass
__version__ = '1.1.1'

def version():
    return __version__

class FreeGroup:
    """
    A free group of specified rank.

    >>> from low_index import *
    >>> F4 = FreeGroup(rank=4)
    >>> w = F4('abcDDDdc')
    >>> w
    Word([1, 2, 3, -4, -4, 3], rank=4)
    >>> print(w)
    abcDDc
    >>> v = F4('ababab')
    >>> v*w
    Word([1, 2, 1, 2, 1, 2, 1, 2, 3, -4, -4, 3], rank=4)
    >>> print(v*w)
    ababababcDDc
    """
    groups = {}

    def __new__(cls, *, rank):
        if not isinstance(rank, int) or rank < 1:
            raise ValueError('%s is not a valid rank'%rank)
        if rank not in cls.groups:
            cls.instance = super(FreeGroup, cls).__new__(cls)
            cls.groups[rank] = cls.instance
        return cls.groups[rank]

    def __init__(self, *, rank):
        self.rank = rank

    def __repr__(self):
        return 'FreeGroup(rank=%d)'%self.rank

    def __str__(self):
        return '<free group of rank %d>'%self.rank

    def __call__(self, word):
        return ReducedWord(word, rank=self.rank)
    
