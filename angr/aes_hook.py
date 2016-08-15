import angr
from Crypto.Cipher import AES

global key

# AES ECB 128 hooks for angr

def aes_set_decrypt_key(state):
    global key
    skey = state.memory.load(state.regs.rdi, 64)
    key = state.se.any_str(skey)

def aes_decrypt(state):
    iv = 16 * '\x00'
    mode = AES.MODE_ECB
    decryptor = AES.new(key[:32], mode, IV = iv)
    sdata = state.memory.load(state.regs.rdi, 16)
    data = state.se.any_str(sdata)
    plain = decryptor.decrypt(data[:32])
    state.memory.store(state.regs.rsi, plain)
