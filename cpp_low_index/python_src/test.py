import unittest

from collections import Counter

from cpp_low_index import *

class TestSimsNode(unittest.TestCase):
    def test_doc(self):
        self.assertIn('A non-abstract SimsNode',
                      SimsNode.__doc__)
        self.assertIn('Create SimsNode for a covering graph ',
                      SimsNode.__init__.__doc__)
        self.assertIn('Copy a SimsNode',
                      SimsNode.__init__.__doc__)

    def test_add_edge(self):
        node = SimsNode(8, 4)
        self.assertEqual(node.rank, 8)

        node.add_edge(2, 1, 1)
        node.add_edge(3, 1, 2)
        node.add_edge(4, 2, 3)
        self.assertTrue(node.verified_add_edge(2, 1, 2))
        self.assertTrue(node.verified_add_edge(2, 2, 3))
        self.assertFalse(node.verified_add_edge(2, 3, 1))

        self.assertFalse(node.is_complete())
        self.assertEqual(node.degree, 3)
        self.assertIn('1--( 3)->2', str(node))

        with self.assertRaises(ValueError):
            node.permutation_rep()

    def test_permutation_rep(self):
        node = SimsNode(2, 2)
        node.add_edge(1, 1, 1)
        node.add_edge(2, 1, 2)
        node.add_edge(2, 2, 1)
        node.add_edge(1, 2, 2)
        self.assertTrue(node.is_complete())
        self.assertEqual(node.degree, 2)
        self.assertEqual(node.permutation_rep(), [[0, 1], [1, 0]])

class TestSimsTree(unittest.TestCase):
    def test_basic(self):
        t = SimsTree(2, 2, [], [])

        perm_reps = [ node.permutation_rep() for node in t.list() ]

        self.assertEqual(
            perm_reps,
            [[[0], [0]],
             [[0, 1], [1, 0]],
             [[1, 0], [0, 1]],
             [[1, 0], [1, 0]]])

    def test_figure_eight(self):
        #                             Manifold("m004").fundamental_group()
        #                     a  a  a  b  A  B  B  A  b
        t = SimsTree(2, 6, [[ 1, 1, 1, 2,-1,-2,-2,-1, 2],
                            [ 2, 1, 1, 1, 2,-1,-2,-2,-1],
                            [-1, 2, 1, 1, 1, 2,-1,-2,-2],
                            [-2,-1, 2, 1, 1, 1, 2,-1,-2],
                            [-2,-2,-1, 2, 1, 1, 1, 2,-1],
                            [-1,-2,-2,-1, 2, 1, 1, 1, 2],
                            [ 2,-1,-2,-2,-1, 2, 1, 1, 1],
                            [ 1, 2,-1,-2,-2,-1, 2, 1, 1],
                            [ 1, 1, 2,-1,-2,-2,-1, 2, 1],
                            [ 1, 1, 1, 2,-1,-2,-2,-1, 2]
                            ],
                     [])

        degrees = Counter([cover.degree for cover in t.list()])
        self.assertEqual(degrees[0], 0)
        self.assertEqual(degrees[1], 1)
        self.assertEqual(degrees[2], 1)
        self.assertEqual(degrees[3], 1)
        self.assertEqual(degrees[4], 2)
        self.assertEqual(degrees[5], 4)
        self.assertEqual(degrees[6], 11)
        self.assertEqual(degrees[7], 0) # Beyond what we counted

class TestPermutationRep(unittest.TestCase):
    def test_K11n34_7(self):
        reps = permutation_reps(
            3,
            ["aaBcbbcAc"],
            ["aacAbCBBaCAAbbcBc"],
            7)

        degrees = Counter([len(rep[0]) for rep in reps])
        self.assertEqual(degrees[1], 1)
        self.assertEqual(degrees[2], 1)
        self.assertEqual(degrees[3], 1)
        self.assertEqual(degrees[4], 1)
        self.assertEqual(degrees[5], 2)
        self.assertEqual(degrees[6], 16)
        self.assertEqual(degrees[7], 30)

        self.assertIn([[0], [0], [0]], reps)
        self.assertIn([[0, 3, 5, 4, 1, 2], [1, 0, 5, 2, 4, 3], [1, 4, 0, 3, 5, 2]], reps)
        
    def test_K15n12345_7(self):
        reps = permutation_reps(
            3,
            ["aBcACAcb"],
            ["aBaCacBAcAbaBabaCAcAbaBaCacBAcAbaBabCAcAbABaCabABAbABaCabCAcAb"],
            7)

        degrees = Counter([len(rep[0]) for rep in reps])
        self.assertEqual(degrees[1], 1)
        self.assertEqual(degrees[2], 1)
        self.assertEqual(degrees[3], 1)
        self.assertEqual(degrees[4], 1)
        self.assertEqual(degrees[5], 3)
        self.assertEqual(degrees[6], 11)
        self.assertEqual(degrees[7], 22)

        self.assertIn([[0], [0], [0]], reps)
        self.assertIn([[0, 2, 1, 3, 4, 6, 5], [0, 3, 4, 5, 1, 2, 6], [1, 2, 0, 5, 6, 3, 4]], reps)
        
    def test_o9_15405_9(self):
        reps = permutation_reps(
            2,
            [],
            ["aaaaabbbaabbbaaaaabbbaabbbaaaaaBBBBBBBB"],
            9)

        degrees = Counter([len(rep[0]) for rep in reps])
        self.assertEqual(degrees[1], 1)
        self.assertEqual(degrees[2], 1)
        self.assertEqual(degrees[3], 1)
        self.assertEqual(degrees[4], 1)
        self.assertEqual(degrees[5], 3)
        self.assertEqual(degrees[6], 3)
        self.assertEqual(degrees[7], 9)
        self.assertEqual(degrees[8], 5)
        self.assertEqual(degrees[9], 14)

        self.assertIn([[0], [0]], reps)
        self.assertIn([[0, 2, 4, 1, 5, 3], [1, 0, 5, 4, 2, 3]], reps)

    def _test_o9_03127_9(self, use_long):
        # Relator is longer than 255 and thus tests that we do not accidentally
        # use a 8-bit integer to index into the relator.
        
        relator = "aabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbbaabbbaaabbbaabbbaabbaabbb"
        
        reps = permutation_reps(
            2,
            [] if use_long else [ relator ],
            [ relator ] if use_long else [],
            4)

        degrees = Counter([len(rep[0]) for rep in reps])
        
        self.assertEqual(degrees[1], 1)
        self.assertEqual(degrees[2], 3)
        self.assertEqual(degrees[3], 2)
        self.assertEqual(degrees[4], 8)

        self.assertIn([[0, 1, 3, 2], [1, 3, 0, 2]], reps)
        
    def test_o9_03127_9_short(self):
        self._test_o9_03127_9(False)

    def test_o9_03127_9_long(self):
        self._test_o9_03127_9(True)

if __name__ == '__main__':
    unittest.main()


