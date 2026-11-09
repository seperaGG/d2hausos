#!/usr/bin/env python3
"""mklonglib.py DLL OUT.lib NAME:HINT ... -- an old-style (long format) import library.

D2Gdi's smackw32.lib is not a lib.exe 6.00 import library: the original's Rich header counts its 8 members as
untagged objects (no @comp.id; 6 imports + descriptor + null thunk, as D2DDraw 8 and D2Glide 45 also show), and the
hint/name table holds the decorated symbol itself (`_SmackWait@4`, hint = the export's index in smackw32.dll). This
writes such a library: one COFF object per import (.text stub `jmp [__imp_X]`, .idata$5 and .idata$4 slots, the
.idata$6 hint/name), the import descriptor object (.idata$2 + the DLL name in .idata$6) and the null thunk object
(.idata$5/.idata$4 terminators), with the two linker members, in MSCMS.LIB's layout (VC98/LIB: an old-format
import library on the VC6 disc; its .debug$S records are left out). Member order: descriptor, null descriptor,
null thunk, then the imports. The __NULL_IMPORT_DESCRIPTOR the link uses comes from the first library on the
line (lib.exe's Storm.lib), as the original's Linker600 count shows.

NAME is the symbol as the code references it without the leading underscore (SmackWait@4); HINT is decimal.
Member order = argument order.
"""
import struct
import sys
import time

DIR32, DIR32NB = 6, 7
COMDAT = 0x1000
TEXT = 0x60200020                       # code, execute, read, align 2
IDATA = 0xC0300040                      # initialised data, read, write, align 4
IDATA6 = 0xC0200040                     # align 2
SECDEF = 0xC0000040                     # the value of an IMAGE_SYM_CLASS_SECTION symbol (as MSCMS.LIB has it)
STAMP = 0


def coff(sections, symbols):
    """sections: [(name, flags, data, [(offset, symindex, type)])];
    symbols: [(name, value, secnum, type, cls, aux bytes or b'')] in table order (indices counted with aux)."""
    strtab = b''

    def name8(n):
        nonlocal strtab
        n = n.encode('latin-1')
        if len(n) <= 8:
            return n.ljust(8, b'\0')
        off = 4 + len(strtab)
        strtab += n + b'\0'
        return struct.pack('<II', 0, off)
    syms = b''
    nsyms = 0
    for (n, val, sec, typ, cls, aux) in symbols:
        syms += name8(n) + struct.pack('<IhHBB', val, sec, typ, cls, len(aux) // 18) + aux
        nsyms += 1 + len(aux) // 18
    pos = 20 + 40 * len(sections)
    body = b''
    shdrs = b''
    for (n, fl, data, rel) in sections:
        raw = pos if data else 0
        pos += len(data)
        rp = pos if rel else 0
        relb = b''.join(struct.pack('<IIH', o, si, t) for (o, si, t) in rel)
        pos += len(relb)
        shdrs += n.encode().ljust(8, b'\0') + struct.pack('<IIIIIIHHI', 0, 0, len(data), raw, rp, 0, len(rel), 0, fl)
        body += data + relb
    fh = struct.pack('<HHIIIHH', 0x14c, len(sections), STAMP, pos, nsyms, 0, 0)
    return fh + shdrs + body + syms + struct.pack('<I', 4 + len(strtab)) + strtab


def secaux(length, nrel, number=0, selection=0):
    return struct.pack('<IHHIHB', length, nrel, 0, 0, number, selection) + b'\0\0\0'


def import_member(dll, name, hint):
    """as MSCMS.LIB's members: COMDAT .text stub and .idata$5 slot (no duplicates), .idata$4 and .idata$6
    associative to .idata$5; the hint/name carries the decorated symbol (the original's `_SmackWait@4`)."""
    sym = '_' + name
    hn = struct.pack('<H', hint) + sym.encode() + b'\0'
    if len(hn) & 1:
        hn += b'\0'
    stub = b'\xff\x25\0\0\0\0'
    secs = [('.text', TEXT | COMDAT, stub, [(2, 5, DIR32)]),
            ('.idata$5', IDATA | COMDAT, b'\0' * 4, [(0, 8, DIR32NB)]),
            ('.idata$4', IDATA | COMDAT, b'\0' * 4, [(0, 8, DIR32NB)]),
            ('.idata$6', IDATA6 | COMDAT, hn, [])]
    syms = [('.text', 0, 1, 0, 3, secaux(6, 1, 0, 1)),               # 0
            (sym, 0, 1, 0x20, 2, b''),                               # 2
            ('.idata$5', 0, 2, 0, 3, secaux(4, 1, 0, 1)),            # 3
            ('__imp_' + sym, 0, 2, 0, 2, b''),                       # 5
            ('.idata$4', 0, 3, 0, 3, secaux(4, 1, 2, 5)),            # 6
            ('.idata$6', 0, 4, 0, 3, secaux(len(hn), 0, 2, 5)),      # 8
            ('__IMPORT_DESCRIPTOR_' + dll, 0, 0, 0, 2, b'')]         # 10
    return coff(secs, syms), [sym, '__imp_' + sym]


def descriptor_member(dll, dllfile):
    """the import descriptor: .idata$2 points at the DLL's grouped .idata$4 and .idata$5 through section-class
    symbols (IMAGE_SYM_CLASS_SECTION, undefined), and at its name in .idata$6"""
    nm = dllfile.encode() + b'\0'
    if len(nm) & 1:
        nm += b'\0'
    secs = [('.idata$2', IDATA, b'\0' * 20, [(12, 2, DIR32NB), (0, 3, DIR32NB), (16, 4, DIR32NB)]),
            ('.idata$6', IDATA6, nm, [])]
    desc = '__IMPORT_DESCRIPTOR_' + dll
    syms = [(desc, 0, 1, 0, 2, b''),                                 # 0
            ('.idata$2', SECDEF, 1, 0, 104, b''),                    # 1
            ('.idata$6', 0, 2, 0, 3, b''),                           # 2
            ('.idata$4', SECDEF, 0, 0, 104, b''),                    # 3
            ('.idata$5', SECDEF, 0, 0, 104, b''),                    # 4
            ('__NULL_IMPORT_DESCRIPTOR', 0, 0, 0, 2, b''),           # 5
            ('\x7f' + dll + '_NULL_THUNK_DATA', 0, 0, 0, 2, b'')]    # 6
    return coff(secs, syms), [desc]


def null_descriptor_member():
    return coff([('.idata$3', IDATA, b'\0' * 20, [])],
                [('__NULL_IMPORT_DESCRIPTOR', 0, 1, 0, 2, b'')]), ['__NULL_IMPORT_DESCRIPTOR']


def thunk_member(dll):
    n = '\x7f' + dll + '_NULL_THUNK_DATA'
    return coff([('.idata$5', IDATA, b'\0' * 4, []), ('.idata$4', IDATA, b'\0' * 4, [])],
                [(n, 0, 1, 0, 2, b'')]), [n]


def archive(members):
    """members: [(member name, bytes, [public symbols])]"""
    longnames = b''
    names = []
    for (mn, _, _) in members:
        b = (mn + '/').encode()
        if len(b) <= 16:
            names.append(b.ljust(16))
        else:
            names.append(('/%d' % len(longnames)).encode().ljust(16))
            longnames += mn.encode() + b'\0'

    def hdr(name, size):
        return name + b'%-12d' % STAMP + b'      ' + b'      ' + b'100666  ' + b'%-10d' % size + b'`\n'
    allsyms = [(s, i) for i, (_, _, ss) in enumerate(members) for s in ss]
    n = len(allsyms)
    first_size = 4 + 4 * n + sum(len(s) + 1 for s, _ in allsyms)
    second_size = 4 + 4 * len(members) + 4 + 2 * n + sum(len(s) + 1 for s, _ in allsyms)

    def pad(b):
        return b + (b'\n' if len(b) & 1 else b'')
    off = 8 + 60 + first_size + (first_size & 1) + 60 + second_size + (second_size & 1)
    if longnames:
        off += 60 + len(longnames) + (len(longnames) & 1)
    offsets = []
    for (mn, data, _) in members:
        offsets.append(off)
        off += 60 + len(data) + (len(data) & 1)
    first = struct.pack('>I', n) + b''.join(struct.pack('>I', offsets[i]) for _, i in allsyms) + \
        b''.join(s.encode('latin-1') + b'\0' for s, _ in allsyms)
    srt = sorted(allsyms, key=lambda x: x[0].encode('latin-1'))
    second = struct.pack('<I', len(members)) + b''.join(struct.pack('<I', o) for o in offsets) + \
        struct.pack('<I', n) + b''.join(struct.pack('<H', i + 1) for _, i in srt) + \
        b''.join(s.encode('latin-1') + b'\0' for s, _ in srt)
    out = b'!<arch>\n' + hdr(b'/'.ljust(16), len(first)) + pad(first) + hdr(b'/'.ljust(16), len(second)) + \
        pad(second)
    if longnames:
        out += hdr(b'//'.ljust(16), len(longnames)) + pad(longnames)
    for (mn, data, _), nm in zip(members, names):
        out += hdr(nm, len(data)) + pad(data)
    return out


def main():
    dllfile, out = sys.argv[1], sys.argv[2]
    dll = dllfile.rsplit('.', 1)[0]
    members = []
    d, ds = descriptor_member(dll, dllfile)
    members.append((dllfile, d, ds))
    n, ns = null_descriptor_member()
    members.append((dllfile, n, ns))
    t, ts = thunk_member(dll)
    members.append((dllfile, t, ts))
    for a in sys.argv[3:]:
        name, hint = a.rsplit(':', 1)
        o, ss = import_member(dll, name, int(hint))
        members.append((dllfile, o, ss))
    open(out, 'wb').write(archive(members))


if __name__ == '__main__':
    main()
