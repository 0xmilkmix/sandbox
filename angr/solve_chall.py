#!/usr/bin/env python

import angr
import binascii

def calc_offset(state):
    state.regs.rax = state.se.BVV(137, 64)

b = angr.Project('./chall')
arg1 = angr.claripy.BVS('arg1', 16 * 8)
angr.path_group.l.setLevel('DEBUG')
b.hook(0x00400bdc, calc_offset, length = 5)
initial_state = b.factory.entry_state(args=['./chall', arg1])
initial_state.add_constraints(arg1.get_byte(11) == angr.claripy.BVV(0x2a, 8))
initial_state.add_constraints(arg1.get_byte(12) == angr.claripy.BVV(0x41, 8))
initial_state.add_constraints(arg1.get_byte(15) == angr.claripy.BVV(0x99, 8))
for c in arg1.chop(8):
    initial_state.add_constraints(c != 0)
pg = b.factory.path_group(initial_state)
pg.explore(find=0x00400c0f, avoid=[0x00400c05, 0x00400bbd, 0x00400b3c, 0x00400b12, 0x00400ad3])
print pg
if len(pg.found) == 0:
    exit(1)
found = pg.found[0]
flag = found.state.se.any_str(arg1)
print binascii.hexlify(flag)

# should be python -c "print 'insomnihack*A\x0b\x16\x99'"
