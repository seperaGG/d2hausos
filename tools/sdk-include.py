#!/usr/bin/env python3
"""sdk-include.py CABDIR OUTDIR -- rebuild the Platform SDK April 2000 include directory.

The SDK disc keeps its files in psdk-common.*.cab under MSI file keys ("WinSock_h.95CE7D8C_A68D_..."), not under
their names. Extract every cab (cabextract -d CABDIR ...), then run this: each key ending in _h/_inl/_idl/_hxx/_hpp
becomes <name>.<ext> (lower case; the tools are case-insensitive). Where several keys give one name (mostly sample
headers), the copy whose key belongs to the core include component (GUID prefixes of windows.h and friends) wins,
then the newest file date (cabextract keeps the cab's dates), then the key name - deterministic.
"""
import collections, os, re, shutil, sys

src, dst = sys.argv[1], sys.argv[2]
os.makedirs(dst, exist_ok=True)
names = collections.defaultdict(list)
for f in os.listdir(src):
	key, _, guid = f.partition('.')
	m = re.match(r'(.+)_(h|inl|idl|hxx|hpp)$', key, re.I)
	if m:
		names[(m.group(1) + '.' + m.group(2)).lower()].append(f)
core = {f.partition('.')[2][:6] for n in ('windows.h', 'winbase.h', 'winsock.h', 'winuser.h', 'ddraw.h', 'dsound.h')
	for f in names[n]}
for n, fs in names.items():
	pick = [f for f in fs if f.partition('.')[2][:6] in core] or fs
	best = sorted(pick, key=lambda f: (-os.stat(os.path.join(src, f)).st_mtime, f))[0]
	shutil.copy2(os.path.join(src, best), os.path.join(dst, n))
print(len(names), 'headers ->', dst)
