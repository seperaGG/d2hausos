#!/bin/sh
# Build the Diablo II 1.00 modules byte-identical to the originals into out/, and check each sha256.
set -e
cd "$(dirname "$0")"
T="sh $(pwd)/tools/msvc.sh"
mkdir -p out/D2Net out/D2Lang

# D2Net.dll
cd D2Net
O=../out/D2Net
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /I../include /Fo$O/ Client.cpp D2Net.cpp Server.cpp
for m in Fog Storm; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Net.ilk ../out/D2Net.dll
LINK_TIME="2000-05-26 18:58:27" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Net.def /OUT:$O/D2Net.dll $O/Client.obj $O/D2Net.obj $O/Server.obj $O/Storm.lib wsock32.lib \
	kernel32.lib $O/Fog.lib
cp $O/D2Net.dll ../out/D2Net.dll
cd ..

# D2Lang.dll
cd D2Lang
O=../out/D2Lang
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I../include /Fo$O/"
$C /O2 /Ob2 D2Lang.cpp Memory.cpp STRSUP.CPP TEXTSUP.CPP strtable.cpp
$C /O2 UNICASE.CPP UNISYS.CPP UNIUTF.CPP
$C /O1 /Ob2 Archive.cpp
for m in Fog Storm; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/Archive.obj >/dev/null
rm -f $O/D2Lang.ilk ../out/D2Lang.dll
LINK_TIME="2000-05-26 18:53:13" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Lang.def /OUT:$O/D2Lang.dll $O/D2Lang.obj $O/Memory.obj $O/STRSUP.obj $O/TEXTSUP.obj $O/UNICASE.obj \
	$O/UNISYS.obj $O/UNIUTF.obj $O/strtable.obj $O/Storm.lib $O/D2Hell.lib kernel32.lib advapi32.lib $O/Fog.lib
cp $O/D2Lang.dll ../out/D2Lang.dll
cd ..

sha256sum -c <<EOF
7876cc8dfe779102f1218a5c7352a6962c95e366ddd524e454332faf9554d0fc  out/D2Net.dll
74ae0debb325048a1ae76f8360f5a826203fa3627fc02f8a62aaa3ae0d305a4b  out/D2Lang.dll
EOF
