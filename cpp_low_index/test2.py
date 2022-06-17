"""

Test script.

Assumes build with::

  python setup.py build --build-lib lib_dev

"""

import sys
sys.path.insert(0, './lib_dev')
import cpp_low_index

#                                   a  a  a  b  A  B  B  A  b
t = cpp_low_index.SimsTree(2, 6, [[ 1, 1, 1, 2,-1,-2,-2,-1, 2],
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


#t.pyList()

l = t.list(1000, 10)

degree_dict = {}
for x in l:
    degree_dict[x.degree] = degree_dict.setdefault(x.degree, 0) + 1

print("==============")

for x in l:
    print(x)
    print(x.degree)

print(degree_dict)

degree_dict_baseline = { 1 : 1,
                         2 : 1,
                         3 : 1,
                         4 : 2,
                         5 : 4,
                         6 : 11 }
if degree_dict != degree_dict_baseline:
    raise Exception("Wrong number of covers")
