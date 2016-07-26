#!/usr/bin/env python

import angr
import binascii

b = angr.Project('./simple')
initial_state = b.factory.entry_state(args = ['./simple'])
c = initial_state.se.state.posix.files[0].read_from(1)
initial_state.add_constraints(c == angr.claripy.BVV('z', 8))
pg = b.factory.path_group(initial_state)
pg.explore(find = 0x00400785, avoid = 0x00400686)
print pg
if len(pg.found) == 0:
    exit(1)
found = pg.found[0]
flag = found.state.posix.dumps(0).strip('\0\n')
print flag
