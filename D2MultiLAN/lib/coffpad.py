#!/usr/bin/env python3
"""coffpad.py OBJ [-s FUNC:N] [-l FUNC:OFFSET] ... : debug padding of a VC6 COFF object.

A /DEBUGTYPE:COFF image keeps, in the aux record of every static function, two numbers that are positions inside the
original OBJECT file: the symbol index of its .bf (and of the next function), and the file offset of its line
records. The original objects were about ten times ours (the real headers' CodeView records), so neither can come
from our source. This tool pads our object until both equal the original's:

  -s FUNC:N        insert N `$L` label symbols (class LABEL, in the previous function's section; the linker copies no
                   labels into the image) just before FUNC's .text section symbol; every later symbol index (relocations,
                   function / .bf aux records) is renumbered.
  -l FUNC:OFFSET   move FUNC's line records to file offset OFFSET (decimal or 0x...) by padding the last .debug$S
                   section written before FUNC's .text: zero bytes added at the end of its last records, inside their
                   length fields (CodeView readers step from record to record by the length), no record grown past
                   4096 bytes; relocations behind a grown record move with it. Measured: LINK 6.00 fails in Pass2
                   ("Internal error", C0000005) on an S_SKIP record (0x0007) and on records grown to 64 KB.
                   OFFSET must not be below the natural offset.
  -L FUNC:OFFSET   the same, padding the object's first .debug$S (the file-level one: types, constants, globals).
  -f FUNC          write FUNC's .debug$F section (its FPO record, 16 bytes and a relocation) after all other sections,
                   for a stretch between two functions' line records that is longer in ours than in the original (the
                   COFF format places no order on section data; the linker reads each by its pointer).

-s apply first, then -f, then -l (give -l in object order). Sections are rewritten in their order with
their raw data, relocations and line numbers back to back (cl's own layout); the object is overwritten in place.
"""
import sys, struct

def u16(b, o): return struct.unpack_from('<H', b, o)[0]
def u32(b, o): return struct.unpack_from('<I', b, o)[0]

class Obj:
    def __init__(self, d):
        self.hdr = bytearray(d[:20])
        nsec, st, ns = u16(d, 2), u32(d, 8), u32(d, 12)
        self.secs = []
        for k in range(nsec):
            h = bytearray(d[20 + k * 40:60 + k * 40])
            size, praw, prel, pln, nrel, nln = u32(h, 16), u32(h, 20), u32(h, 24), u32(h, 28), u16(h, 32), u16(h, 34)
            raw = bytearray(d[praw:praw + size]) if praw else None
            rel = bytearray(d[prel:prel + nrel * 10]) if nrel else b''
            ln = bytearray(d[pln:pln + nln * 6]) if nln else b''
            self.secs.append([h, raw, bytearray(rel), bytearray(ln)])
        self.syms = [bytearray(d[st + i * 18:st + i * 18 + 18]) for i in range(ns)]
        self.strtab = bytearray(d[st + ns * 18:]); self.moved = []

    def name(self, i):
        r = self.syms[i][:8]
        if r[:4] == b'\0\0\0\0':
            o = u32(r, 4); return self.strtab[o:self.strtab.index(b'\0', o)].decode('latin1')
        return bytes(r).rstrip(b'\0').decode('latin1')

    def entries(self):
        """(index, symbol) of every primary symbol"""
        i = 0
        while i < len(self.syms):
            yield i, self.syms[i]; i += 1 + self.syms[i][17]

    def func(self, fname):
        for i, s in self.entries():
            if u16(s, 14) == 0x20 and struct.unpack_from('<h', s, 12)[0] > 0 and self.name(i) == fname:
                return i, struct.unpack_from('<h', s, 12)[0]
        sys.exit('coffpad: no function %s' % fname)

    def insert_labels(self, fname, n):
        fi, sec = self.func(fname)
        at = next(i for i, s in self.entries() if self.name(i) == '.text' and struct.unpack_from('<h', s, 12)[0] == sec)
        prev = max(struct.unpack_from('<h', s, 12)[0] for i, s in self.entries()
                   if i < at and self.name(i) == '.text' and s[16] == 3)
        def fix(v): return v + n if v >= at else v
        for i, s in self.entries():
            if s[17] and u16(s, 14) == 0x20:                       # function aux: .bf index, next function
                a = self.syms[i + 1]; struct.pack_into('<I', a, 0, fix(u32(a, 0)))
                if u32(a, 12): struct.pack_into('<I', a, 12, fix(u32(a, 12)))
            elif s[17] and self.name(i) == '.bf':                  # .bf aux: next .bf
                a = self.syms[i + 1]
                if u32(a, 12): struct.pack_into('<I', a, 12, fix(u32(a, 12)))
        for h, raw, rel, ln in self.secs:
            for k in range(0, len(rel), 10): struct.pack_into('<I', rel, k + 4, fix(u32(rel, k + 4)))
            for k in range(0, len(ln), 6):
                if u16(ln, k + 4) == 0: struct.pack_into('<I', ln, k, fix(u32(ln, k)))   # a function's line record 0
        pad = [bytearray(struct.pack('<8sIhHBB', ('$LPAD%02d' % k).encode(), 0, prev, 0, 6, 0)) for k in range(n)]
        self.syms[at:at] = pad

    def order(self):
        """section indices in file order: cl's order, the moved ones last"""
        return [k for k in range(len(self.secs)) if k not in self.moved] + self.moved

    def layout(self):
        o = 20 + 40 * len(self.secs); offs = [None] * len(self.secs)
        for k in self.order():
            h, raw, rel, ln = self.secs[k]
            praw = o if raw else 0; o += len(raw) if raw else 0
            prel = o if rel else 0; o += len(rel)
            pln = o if ln else 0; o += len(ln)
            offs[k] = (praw, prel, pln)
        return offs, o

    def move_fpo(self, fname):
        fi, sec = self.func(fname)
        k = next(j for j in range(sec, len(self.secs)) if bytes(self.secs[j][0][:8]).rstrip(b'\0') == b'.debug$F')
        self.moved.append(k)

    def pad_lines(self, fname, target, first=False):
        fi, sec = self.func(fname)
        offs, _ = self.layout(); cur = offs[sec - 1][2]
        n = target - cur
        if n < 0: sys.exit('coffpad: %s lines at %#x, above the target %#x' % (fname, cur, target))
        dbg = [j for j in range(sec - 1) if bytes(self.secs[j][0][:8]).rstrip(b'\0') == b'.debug$S' and j not in self.moved]
        k = dbg[0] if first else dbg[-1]
        h, raw, rel, ln = self.secs[k]
        recs = []; o = 4 if u32(raw, 0) in (1, 2) else 0            # (offset, length) of every record
        while o < len(raw): recs.append(o); o += 2 + u16(raw, o)
        for r in reversed(recs):                                   # zero bytes inside the last records' lengths
            if not n: break
            m = min(n, max(0, 4096 - u16(raw, r))); end = r + 2 + u16(raw, r)
            struct.pack_into('<H', raw, r, u16(raw, r) + m); raw[end:end] = b'\0' * m; n -= m
            for q in range(0, len(rel), 10):
                if u32(rel, q) >= end: struct.pack_into('<I', rel, q, u32(rel, q) + m)
        if n: sys.exit('coffpad: %s: section %d too small to pad' % (fname, k + 1))
        struct.pack_into('<I', h, 16, len(raw))

    def write(self, path):
        offs, o = self.layout()
        # function aux records point at their line records
        for i, s in self.entries():
            if s[17] and u16(s, 14) == 0x20:
                sec = struct.unpack_from('<h', s, 12)[0]
                if sec > 0: struct.pack_into('<I', self.syms[i + 1], 8, offs[sec - 1][2])
        struct.pack_into('<II', self.hdr, 8, o, len(self.syms))
        out = bytearray(self.hdr)
        for (h, raw, rel, ln), (praw, prel, pln) in zip(self.secs, offs):
            struct.pack_into('<III', h, 20, praw, prel, pln); out += h
        for k in self.order(): h, raw, rel, ln = self.secs[k]; out += (raw or b'') + rel + ln
        for s in self.syms: out += s
        out += self.strtab
        open(path, 'wb').write(out)

def main():
    a = sys.argv[1:]
    if not a: sys.exit(__doc__)
    path = a.pop(0); ob = Obj(open(path, 'rb').read()); ops = list(zip(a[0::2], a[1::2]))
    for f, v in ops:
        if f == '-s': fn, n = v.rsplit(':', 1); ob.insert_labels(fn, int(n))
    for f, v in ops:
        if f == '-f': ob.move_fpo(v)
    for f, v in ops:
        if f in ('-l', '-L'): fn, t = v.rsplit(':', 1); ob.pad_lines(fn, int(t, 0), f == '-L')
    ob.write(path)

if __name__ == '__main__': main()
