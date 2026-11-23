#!/usr/bin/env python3
"""mkico.py D2SERVER.DLL OUTDIR : write the icon of Diablo II 1.00's D2Server.dll (group "IDI_APP") as
OUTDIR/D2ServerApp.ico for D2Server.rc. The icon is game art and is not in the repository."""
import struct, sys

d = open(sys.argv[1], 'rb').read()
pe = struct.unpack_from('<I', d, 60)[0]
nsec, opt = struct.unpack_from('<H', d, pe + 6)[0], struct.unpack_from('<H', d, pe + 20)[0]
rsrc = struct.unpack_from('<I', d, pe + 24 + 96 + 8 * 2)[0]
secs = [struct.unpack_from('<IIII', d, pe + 24 + opt + 40 * i + 8) for i in range(nsec)]

def off(rva):
    for vsize, va, rsize, raw in secs:
        if va <= rva < va + max(vsize, rsize):
            return rva - va + raw
    raise SystemExit('mkico.py: bad resource address')

base = off(rsrc)

def entries(o):
    named, ids = struct.unpack_from('<HH', d, base + o + 12)
    for i in range(named + ids):
        name, to = struct.unpack_from('<II', d, base + o + 16 + 8 * i)
        if name & 0x80000000:
            p = base + (name & 0x7fffffff)
            n = struct.unpack_from('<H', d, p)[0]
            name = d[p + 2:p + 2 + 2 * n].decode('utf-16le')
        yield name, to & 0x7fffffff

res = {}
for t, o1 in entries(0):
    for n, o2 in entries(o1):
        for l, o3 in entries(o2):
            rva, size = struct.unpack_from('<II', d, base + o3)
            res[(t, n)] = d[off(rva):off(rva) + size]
g = res[(14, 'IDI_APP')]
_, typ, cnt = struct.unpack_from('<HHH', g, 0)
ents, imgs, pos = [], [], 6 + 16 * cnt
for k in range(cnt):
    w, h, cc, rv, planes, bits, size, iid = struct.unpack_from('<BBBBHHIH', g, 6 + 14 * k)
    img = res[(3, iid)]
    ents.append(struct.pack('<BBBBHHII', w, h, cc, rv, planes, bits, len(img), pos))
    imgs.append(img)
    pos += len(img)
open('%s/D2ServerApp.ico' % sys.argv[2], 'wb').write(struct.pack('<HHH', 0, typ, cnt) + b''.join(ents) + b''.join(imgs))
