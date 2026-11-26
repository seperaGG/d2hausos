#!/bin/sh
# One-time setup: download the five files listed in README.md (or use copies already in downloads/) and unpack
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
SDK99=1_PLATSDK.iso
SDK99_URL=https://archive.org/download/msdn-disc6-october-1999-x05-05207/$SDK99
SDK99_SHA=6ebf3a588079468faded09f1877296887e1d18d26a4298caabfb26a0fce1221b
SHELL32_SHA=b7f179e3d64e7a04098bd1b0f28ce45d59eb253d6d358b038583afdbf56d9b1e
DDRAW99_SHA=d6eace55370ea0ba59acf6b5abe000a8b5c6d2f0935baa2650bb57949fea9801
SP3=msvc6.3.tar.gz
SP3_URL=https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/$SP3
SP3_SHA=84f73e718b3671bfd5de3b7764622b07633b572ee826ca3b77602d224c128608
C2SP3_SHA=a0cc45f83fd0ed009aa4f43df5598105a1049ad9675de0b1089be7cce7d3c153
CVTRESSP3_SHA=83b602ed8e69e979fc9557f482a4a4c6c9a97b4ad67b879aedeacd2b09e5b20b
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
fetch $SDK99 $SDK99_URL $SDK99_SHA
fetch $WIBO $WIBO_URL $WIBO_SHA
fetch $SP3 $SP3_URL $SP3_SHA

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
7z x -y -bd -o"$W/vs6" "$W/vs6.iso" VC98/BIN/CL.EXE VC98/BIN/C1.DLL VC98/BIN/C1XX.DLL VC98/BIN/C2.DLL VC98/BIN/LIB.EXE \
	VC98/BIN/LINK.EXE VC98/BIN/CVTRES.EXE COMMON/MSDEV98/BIN/MSPDB60.DLL COMMON/MSDEV98/BIN/RC.EXE \
	COMMON/MSDEV98/BIN/RCDLL.DLL 'VC98/INCLUDE/*' 'VC98/LIB/*' >/dev/null
rm -f "$W/vs6.iso"
mkdir -p "$TC/bin" "$TC/linkbin"
cp "$W"/vs6/VC98/BIN/* "$W"/vs6/COMMON/MSDEV98/BIN/* "$TC/bin/"
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
cp "$W"/sdk/Version_Lib.* "$TC/sdklib/version.lib"
cp "$W"/sdk/ddraw_Lib.* "$TC/sdklib/ddraw.lib"
cp "$W"/sdk/dsound_Lib.* "$TC/sdklib/dsound.lib"
cp "$W"/sdk/dxguid_Lib.* "$TC/sdklib/dxguid.lib"
cp "$W"/sdk/WinMM_Lib.* "$TC/sdklib/winmm.lib"
cp "$TC"/bin/LINK.EXE "$TC"/bin/CVTRES.EXE "$TC"/bin/MSPDB60.DLL "$TC/linkbin/"
cp "$W"/sdk/Msvcrt_Dll.* "$TC/linkbin/msvcrt.dll"
echo "$MSVCRT_SHA  $TC/linkbin/msvcrt.dll" | sha256sum -c --quiet

# Platform SDK, September 1999: only its shell32 import library (D2gfx; SHAppBarMessage's hint as the original's)
# and its ddraw import library (D2VidTst; the hints as the original's).
echo "unpacking shell32.lib and ddraw.lib of the September 1999 Platform SDK"
7z e -y -bd -o"$W/cab99" "$DL/$SDK99" Setup/psdk-x86.13.cab Setup/psdk-x86.10.cab >/dev/null
cabextract -q -d "$W/sdk99" -F 'Shell32_Lib.*' "$W/cab99/psdk-x86.13.cab"
cabextract -q -d "$W/sdk99" -F 'ddraw_Lib.*' "$W/cab99/psdk-x86.10.cab"
mkdir -p "$TC/sdklib1999"
cp "$W"/sdk99/Shell32_Lib.* "$TC/sdklib1999/shell32.lib"
cp "$W"/sdk99/ddraw_Lib.* "$TC/sdklib1999/ddraw.lib"
echo "$SHELL32_SHA  $TC/sdklib1999/shell32.lib" | sha256sum -c --quiet
echo "$DDRAW99_SHA  $TC/sdklib1999/ddraw.lib" | sha256sum -c --quiet

# Visual C++ 6.0 SP3's back end (c2.dll 12.00.8447) and cvtres (5.00.1736), for D2VidTst: binsp3 is a copy of
# bin with SP3's C2.DLL (cl and c1xx stay RTM) and SP3's CVTRES.EXE.
echo "unpacking the VC6 SP3 back end and cvtres"
mkdir -p "$W/sp3"
tar -xzf "$DL/$SP3" -C "$W/sp3" Bin/c2.dll Bin/CVTRES.EXE
echo "$C2SP3_SHA  $W/sp3/Bin/c2.dll" | sha256sum -c --quiet
echo "$CVTRESSP3_SHA  $W/sp3/Bin/CVTRES.EXE" | sha256sum -c --quiet
cp -r "$TC/bin" "$TC/binsp3"
rm -f "$TC/binsp3/C2.DLL" "$TC/binsp3/CVTRES.EXE"
cp "$W/sp3/Bin/c2.dll" "$TC/binsp3/C2.DLL"
cp "$W/sp3/Bin/CVTRES.EXE" "$TC/binsp3/CVTRES.EXE"
rm -rf "$W"

# A Wine prefix for the linker (no Mono/Gecko prompts).
echo "creating Wine prefix"
WINEPREFIX=$TC/wineprefix WINEDEBUG=-all WINEDLLOVERRIDES="mscoree,mshtml=" wineboot -i >/dev/null 2>&1
echo "setup done; now run: sh build.sh"
