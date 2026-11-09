#!/bin/sh
# Run a Visual C++ 6.0 RTM tool from ../toolchain (made by ../setup.sh):
#   tools/msvc.sh cl|lib|link <arguments>
# cl and lib run under wibo. link runs under Wine, because wibo cannot do an incremental link, and on Microsoft's
# msvcrt.dll, because Wine's own qsort orders the import table differently.
# LINK_TIME="YYYY-MM-DD hh:mm:ss" (UTC) runs the linker on a frozen clock (libfaketime), so it writes the
# original timestamps itself.
set -e
TC=$(cd "$(dirname "$0")/.." && pwd)/toolchain
tool=$1; shift
case $tool in
	cl)   exe=CL.EXE ;;
	lib)  exe=LIB.EXE ;;
	link) exe=LINK.EXE ;;
	*) echo "msvc.sh: unknown tool $tool" >&2; exit 2 ;;
esac
if [ "$tool" != link ]; then
	export INCLUDE="$TC/sdkinclude;$TC/include" LIB="$TC/sdklib;$TC/lib"
	exec "$TC/wibo" "$TC/bin/$exe" "$@"
fi
export WINEPREFIX=$TC/wineprefix WINEDEBUG=-all
export INCLUDE="$(winepath -w "$TC/sdkinclude");$(winepath -w "$TC/include")"
export LIB="$(winepath -w "$TC/sdklib");$(winepath -w "$TC/lib")"
export WINEDLLOVERRIDES="msvcrt=n"
if [ -n "$LINK_TIME" ]; then
	# A wineserver started under the frozen clock hangs on exit, so one is started first on the real clock.
	WS=$(command -v wineserver || ls /usr/lib/wine/wineserver /usr/lib/x86_64-linux-gnu/wine/wineserver \
		/usr/lib/i386-linux-gnu/wine/wineserver 2>/dev/null | head -n 1 || true)
	if [ -n "$WS" ]; then "$WS" -p 5 || true; else WINEDLLOVERRIDES= wine cmd /c exit >/dev/null 2>&1 || true; fi
	exec env TZ=UTC faketime -f "$LINK_TIME" wine "$TC/linkbin/$exe" "$@"
fi
exec wine "$TC/linkbin/$exe" "$@"
