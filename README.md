# Diablo II Hausos

Decompiled source code of Diablo II 1.00, reconstructed from the original binaries, that builds to modules
byte-for-byte identical to the originals. A module is added
here once its build matches the original; the modules so far:

| file | size | sha256 |
|---|---|---|
| `D2Net.dll` | 49,152 | `7876cc8dfe779102f1218a5c7352a6962c95e366ddd524e454332faf9554d0fc` |
| `D2Lang.dll` | 110,592 | `74ae0debb325048a1ae76f8360f5a826203fa3627fc02f8a62aaa3ae0d305a4b` |
| `D2MCPClient.dll` | 53,248 | `2dba706661c163302c3900b1a640b6136f8954d6d3fc735d6882fc1331bfc523` |
| `D2Gdi.dll` | 57,344 | `84511be68e9f25c805ea755c1444544338d093a0348ccfa30d2dd8ce9ab907a8` |
| `D2DDraw.dll` | 118,784 | `2650e8a49d6a8c09fe1f3c3425873e94a9b3125ea95de98ba447e025940d9cbb` |
| `D2sound.dll` | 143,360 | `d403adb406b93c76c8c0d1bea131b648754fca3a8337120314bfe3e0f6e3b06c` |
| `D2Launch.dll` | 212,992 | `94ec7c4a04e5020aa5eaca8a0a80904393ce774af8315294a55fc6385e6a1e6c` |
| `D2Multi.dll` | 184,320 | `5dccb996981f60ac2bc5fcaea4e9f033dc1c1ccb4fab9b8d608b76de9fa7fd4f` |
| `D2Glide.dll` | 139,264 | `34bcf5a35fd5197dd859ab0d704460ae007d16095f7693f48d47a7db88cda381` |
| `D2Direct3D.dll` | 172,032 | `1a070bb5c8ee4d159a333a1abd0cad5608ede2fa12cdd70e14e3e1b1e96291b1` |
| `D2Game.dll` | 802,816 | `695024b9f2c9f0e9446a1b224d823d4a8d5f9424b6cc314953f01a5a34eaebc4` |
| `D2Common.dll` | 724,992 | `6e8b73325ec6b885213ecc9a98017ec5cf6d6984ff4a2735c73a3f3595e0544c` |
| `D2MultiLAN.dll` | 119,112 | `45b2a54d4108cba651a0de5fa165cf99e10be110352bff566b4c30d0cf3aa2d6` |
| `Bnclient.dll` | 159,744 | `5903adc1db1ce25efe5d8609dca9e67430668a5b2035f8cde7a57b28eb9a3cb9` |
| `Fog.dll` | 204,800 | `1c6c1bd1b2fd0ede64dd8035a36bc4f69a3872198268f998ca2bbdd7ae7d8be6` |
| `D2gfx.dll` | 135,168 | `ad23ccc059f07e1cbdef14e30d2ab0ce35d708d6e73af7a35e89d9bea9d08aec` |
| `D2Win.dll` | 196,608 | `0f64dc8bb63d9962face3426d78d645b6b581f33d090d5f9304ae81b23fa0b8c` |
| `D2Server.dll` | 184,320 | `ccaaeb7800376991482e74f4e0e2c01f16088a197d45d38f39eb16ee50c2513b` |
| `D2VidTst.exe` | 98,304 | `2c3d08db2d9595a4b2f3462e53be571436a70c9d883abdb008b170e8d95ecc26` |

## Requirements

Linux x86-64 with: `wine` (10.0 tested, with 32-bit support), `faketime`, `python3` (3.8+), `curl`, `7z`, `cabextract`.

Debian/Ubuntu: `sudo apt install wine wine32:i386 faketime python3 curl p7zip-full cabextract`
(for `wine32:i386`, run `sudo dpkg --add-architecture i386 && sudo apt update` first).

## Build

```sh
sh setup.sh     # one time: downloads ~1.5 GB, unpacks the toolchain into toolchain/
sh build.sh     # builds every module into out/ and checks each sha256
```

`build.sh` ends with one `OK` line per module (`out/D2Net.dll: OK`, ...). Use a path without spaces.

`D2Server.dll` holds one icon, which is not part of this repository. To build it, name a `D2Server.dll` of
Diablo II 1.00 to take the icon from: `D2SERVER_DLL=/path/to/D2Server.dll sh build.sh`. Without it the other
modules are built and D2Server is skipped.

`D2VidTst.exe` likewise holds an icon that is not part of this repository: `D2VIDTST_EXE=/path/to/D2VidTst.exe`
(Diablo II 1.00's) builds it; without it D2VidTst is skipped. Both can be given at once.

## Downloads

`setup.sh` fetches these and checks their sha256. If a download fails (archive.org is sometimes slow or returns
500), download it yourself into `downloads/` under the same file name and run `setup.sh` again.

| what | link | sha256 |
|---|---|---|
| Visual Studio 6.0 Enterprise, disc 1 (cl 12.00.8168, link 6.00.8168) | <https://archive.org/download/X04-62912/1999-02_X04-62912_X04-62912_VSE60I_01.7z> | `7bb1ce5bdd40d3e899371b62b8828d7a400f292346e73d5ecd1c5e9da355a348` |
| Platform SDK, April 2000 (headers, system import libraries, msvcrt.dll) | <https://archive.org/download/x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z/x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z> | `d997b70fac386e46cfde7f509747869f5887cb87b88d09fb3495ee7096b6778f` |
| Platform SDK, September 1999 (MSDN October 1999 disc 6; only its shell32.lib, for D2gfx, and ddraw.lib, for D2VidTst) | <https://archive.org/download/msdn-disc6-october-1999-x05-05207/1_PLATSDK.iso> | `6ebf3a588079468faded09f1877296887e1d18d26a4298caabfb26a0fce1221b` |
| wibo 1.2.0 (runs cl and lib on Linux) | <https://github.com/decompals/wibo/releases/download/1.2.0/wibo-i686> | `2575d3b0a2f408b2c2b0850db56f1af5d005a138394a6774eba77b6708ecc304` |
| Visual C++ 6.0 SP3 back end and cvtres (c2.dll 12.00.8447, cvtres 5.00.1736; decomp.me's msvc6.3 package, for D2VidTst) | <https://github.com/OmniBlade/decomp.me/releases/download/msvcwin9x/msvc6.3.tar.gz> | `84f73e718b3671bfd5de3b7764622b07633b572ee826ca3b77602d224c128608` |
| Intel JPEG Library 1.1 import library `ijl11.lib` (1999-08-05, Intel's redistributable; for D2Win) | <https://raw.githubusercontent.com/Dragon-Raja-Server/Client/f98d0bb049d4d1e607afcdedbcab1b2826ec1c0c/ijl11.lib> | `ba187eb00a12d0d90593cbcd994686a618ab7839f1599f7ab7d16ddc9c3d16b6` |

Item pages: <https://archive.org/details/X04-62912>,
<https://archive.org/details/x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z> (a torrent is offered there too),
<https://archive.org/details/msdn-disc6-october-1999-x05-05207>.

## Contents

| path | what |
|---|---|
| `D2Net/`, `D2Lang/`, `D2MCPClient/`, `D2Gdi/`, `D2DDraw/`, `D2sound/`, `D2Launch/`, `D2Multi/`, `D2Glide/`, `D2Direct3D/`, `D2Game/`, `D2Common/`, `D2MultiLAN/`, `Bnclient/`, `Fog/`, `D2gfx/`, `D2Win/`, `D2Server/`, `D2VidTst/` | one directory per module: the source, export `.def` files, and `lib/*.def` for the import libraries of the game DLLs it calls (D2Gdi, D2DDraw, D2Glide and D2Direct3D also have `lib/mklonglib.py` and `lib/*.exports`, which write their old-format smackw32.lib, binkw32.lib and glide3x.lib; D2Launch has `lib/D2WinSyms.c`, the symbols its D2Win.lib is built from; D2Common and D2Server have `hell/`, the D2Hell static library's files (D2gfx's three are HellReport.cpp, D2gfxMisc.cpp and HellWindow.cpp; D2Win's two HellArchive.cpp and HellBits.cpp); D2Win has `lib/D2gfxSyms.c`, the symbols its D2gfx.lib is built from; D2Common has `lib/renobj.py` and `lib/selfnames.txt`, which rename symbols in copies of its objects before the link; D2MultiLAN has `lib/coffpad.py`, which pads the debug data of its objects before the link; Bnclient has `Bnclient.rc`, its resource (a dialog); D2Server has `D2Server.rc`, its resources, and `lib/mkico.py`, which reads the icon; D2VidTst, an exe, has `D2VidTst.rc` and `lib/mkico.py` likewise, and D2Hell's three files under `D2Hell/SRC/`: Fault.cpp, Window.cpp (the original's names) and Report.cpp) |
| `include/` | headers shared by the modules |
| `setup.sh` | makes `toolchain/` from the downloads |
| `build.sh` | compiles and links every module |
| `LICENSE` | MIT License |
| `tools/msvc.sh` | runs `cl`/`lib` (and `cl-sp3`/`cvtres-sp3`, VC6 SP3's back end and cvtres) under wibo and `link`/`rc` under Wine, with the linker on a fixed clock so it writes the original timestamps |
| `tools/sdk-include.py` | rebuilds the SDK's header directory from its cabinet files |

`toolchain/`, `downloads/` and `out/` are created by the scripts and are not part of the repository.

## License and disclaimer

The source code and scripts in this folder are released under the MIT License; see [`LICENSE`](LICENSE). The
license covers only this project's own files. It does not cover the Microsoft tools and SDK and Intel's `ijl11.lib` that
`setup.sh` downloads, nor any Diablo II game files, which remain the property of their respective owners.

Diablo II Hausos is an unofficial, fan-made project. It is not affiliated with, endorsed, sponsored or approved by
Blizzard Entertainment, Inc. Diablo, Diablo II, Blizzard and Blizzard Entertainment are trademarks or registered
trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries. All other trademarks are the property
of their respective owners.
