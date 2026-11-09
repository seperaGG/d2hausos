#!/bin/sh
# One-time setup: download the three files listed in README.md (or use copies already in downloads/) and unpack
# the toolchain into toolchain/. Needs: curl, 7z, cabextract, python3, wine (with 32-bit support).
set -e
cd "$(dirname "$0")"
HERE=$(pwd)
DL=$HERE/downloads
TC=$HERE/toolchain
W=$TC/.work

VS6=1999-02_X04-62912_X04-62912_VSE60I_01.7z
VS6_URL=https://archive.org/download/X04-62912/$VS6
VS6_SHA=7bb1ce5bdd40d3e899371b62b8828d7a400f292346e73d5ecd1c5e9da355a348
SDK=x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z
SDK_URL=https://archive.org/download/$SDK/$SDK
SDK_SHA=d997b70fac386e46cfde7f509747869f5887cb87b88d09fb3495ee7096b6778f
WIBO=wibo-i686
WIBO_URL=https://github.com/decompals/wibo/releases/download/1.2.0/$WIBO
WIBO_SHA=2575d3b0a2f408b2c2b0850db56f1af5d005a138394a6774eba77b6708ecc304
MSVCRT_SHA=2d5670881a6d13a100e37da297f6eeb876f68b63073a28eb5d650625c60f9bf7

fetch() { # file url sha256
	mkdir -p "$DL"
	if [ ! -f "$DL/$1" ]; then
		echo "downloading $1"
		curl -fL --retry 5 --retry-all-errors -C - -o "$DL/$1.part" "$2"
		mv "$DL/$1.part" "$DL/$1"
	fi
	echo "$3  $DL/$1" | sha256sum -c --quiet || { echo "bad sha256: $DL/$1" >&2; exit 1; }
}
fetch $VS6 $VS6_URL $VS6_SHA
fetch $SDK $SDK_URL $SDK_SHA
fetch $WIBO $WIBO_URL $WIBO_SHA

rm -rf "$TC"; mkdir -p "$W"

# wibo: runs cl and lib on Linux.
cp "$DL/$WIBO" "$TC/wibo"; chmod +x "$TC/wibo"

# Visual C++ 6.0 RTM: the archive holds a raw MDF image (2,448-byte sectors); keep the 2,048 data bytes of each
# sector to get an ISO, then take the compiler, linker, CRT headers and CRT libraries from it.
echo "unpacking Visual C++ 6.0"
7z e -y -bd -o"$W" "$DL/$VS6" '*.mdf' -r >/dev/null
python3 -I -c '
import sys
with open(sys.argv[1], "rb") as i, open(sys.argv[2], "wb") as o:
    while s := i.read(2448):
        o.write(s[16:2064])' "$W"/*.mdf "$W/vs6.iso"
rm -f "$W"/*.mdf
7z x -y -bd -o"$W/vs6" "$W/vs6.iso" VC98/BIN/CL.EXE VC98/BIN/C1XX.DLL VC98/BIN/C2.DLL VC98/BIN/LIB.EXE \
	VC98/BIN/LINK.EXE COMMON/MSDEV98/BIN/MSPDB60.DLL 'VC98/INCLUDE/*' 'VC98/LIB/*' >/dev/null
rm -f "$W/vs6.iso"
mkdir -p "$TC/bin" "$TC/linkbin"
cp "$W"/vs6/VC98/BIN/* "$W/vs6/COMMON/MSDEV98/BIN/MSPDB60.DLL" "$TC/bin/"
mv "$W/vs6/VC98/INCLUDE" "$TC/include"
mv "$W/vs6/VC98/LIB" "$TC/lib"

# Platform SDK, April 2000: its files sit in cabinets under MSI keys ("Kernel32_Lib.<GUID>").
echo "unpacking Platform SDK"
7z e -y -bd -o"$W" "$DL/$SDK" >/dev/null
7z e -y -bd -o"$W/cab" "$W"/*.iso 'Setup/psdk-common.*.cab' 'Setup/psdk-x86.*.cab' >/dev/null
rm -f "$W"/*.iso
cabextract -q -d "$W/sdk" "$W"/cab/*.cab
python3 -I tools/sdk-include.py "$W/sdk" "$W/sdkinc"
# Its headers, minus the CRT-named ones (Visual C++'s win). The SDK's winbase.h is the one the original saw.
CRT="assert.h conio.h ctype.h direct.h dos.h eh.h errno.h excpt.h fcntl.h float.h fstream.h io.h iomanip.h ios.h
iostream.h istream.h limits.h locale.h malloc.h math.h memory.h new.h ostream.h process.h search.h setjmp.h share.h
signal.h stat.h stdarg.h stddef.h stdio.h stdiostr.h stdlib.h string.h strstrea.h time.h typeinfo.h types.h wchar.h
wctype.h"
mkdir -p "$TC/sdkinclude"
for f in "$W"/sdkinc/*; do
	case " $(echo $CRT) " in *" $(basename "$f") "*) continue ;; esac
	mv "$f" "$TC/sdkinclude/"
done
# winsock.h with every prototype __declspec(dllimport) except __WSAFDIsSet, as the original was compiled.
sed -i -E -e 's/^extern (int PASCAL FAR __WSAFDIsSet)/\1/' \
	-e '/__WSAFDIsSet/!s/^([A-Za-z_][A-Za-z_ *]* PASCAL FAR [A-Za-z_]+ *\()/__declspec(dllimport) \1/' \
	"$TC/sdkinclude/winsock.h"
# The system import libraries the DLLs link with, and Microsoft's msvcrt.dll for the linker.
mkdir -p "$TC/sdklib"
cp "$W"/sdk/Kernel32_Lib.* "$TC/sdklib/kernel32.lib"
cp "$W"/sdk/AdvAPI32_Lib.* "$TC/sdklib/advapi32.lib"
cp "$W"/sdk/WSock32_Lib.* "$TC/sdklib/wsock32.lib"
cp "$W"/sdk/User32_Lib.* "$TC/sdklib/user32.lib"
cp "$W"/sdk/Gdi32_Lib.* "$TC/sdklib/gdi32.lib"
cp "$TC"/bin/LINK.EXE "$TC"/bin/MSPDB60.DLL "$TC/linkbin/"
cp "$W"/sdk/Msvcrt_Dll.* "$TC/linkbin/msvcrt.dll"
echo "$MSVCRT_SHA  $TC/linkbin/msvcrt.dll" | sha256sum -c --quiet
rm -rf "$W"

# A Wine prefix for the linker (no Mono/Gecko prompts).
echo "creating Wine prefix"
WINEPREFIX=$TC/wineprefix WINEDEBUG=-all WINEDLLOVERRIDES="mscoree,mshtml=" wineboot -i >/dev/null 2>&1
echo "setup done; now run: sh build.sh"
