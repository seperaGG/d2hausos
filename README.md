# Diablo II Hausos

Source code of Diablo II 1.00 that builds to modules byte-for-byte identical to the originals. A module is added
here once its build matches the original; the modules so far:

| file | size | sha256 |
|---|---|---|
| `D2Net.dll` | 49,152 | `7876cc8dfe779102f1218a5c7352a6962c95e366ddd524e454332faf9554d0fc` |
| `D2Lang.dll` | 110,592 | `74ae0debb325048a1ae76f8360f5a826203fa3627fc02f8a62aaa3ae0d305a4b` |

## Requirements

Linux x86-64 with: `wine` (10.0 tested, with 32-bit support), `faketime`, `python3` (3.8+), `curl`, `7z`, `cabextract`.

Debian/Ubuntu: `sudo apt install wine wine32:i386 faketime python3 curl p7zip-full cabextract`
(for `wine32:i386`, run `sudo dpkg --add-architecture i386 && sudo apt update` first).

## Build

```sh
sh setup.sh     # one time: downloads ~820 MB, unpacks the toolchain into toolchain/
sh build.sh     # builds every module into out/ and checks each sha256
```

`build.sh` ends with one `OK` line per module (`out/D2Net.dll: OK`, ...). Use a path without spaces.

## Downloads

`setup.sh` fetches these and checks their sha256. If a download fails (archive.org is sometimes slow or returns
500), download it yourself into `downloads/` under the same file name and run `setup.sh` again.

| what | link | sha256 |
|---|---|---|
| Visual Studio 6.0 Enterprise, disc 1 (cl 12.00.8168, link 6.00.8168) | <https://archive.org/download/X04-62912/1999-02_X04-62912_X04-62912_VSE60I_01.7z> | `7bb1ce5bdd40d3e899371b62b8828d7a400f292346e73d5ecd1c5e9da355a348` |
| Platform SDK, April 2000 (headers, system import libraries, msvcrt.dll) | <https://archive.org/download/x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z/x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z> | `d997b70fac386e46cfde7f509747869f5887cb87b88d09fb3495ee7096b6778f` |
| wibo 1.2.0 (runs cl and lib on Linux) | <https://github.com/decompals/wibo/releases/download/1.2.0/wibo-i686> | `2575d3b0a2f408b2c2b0850db56f1af5d005a138394a6774eba77b6708ecc304` |

Item pages: <https://archive.org/details/X04-62912>,
<https://archive.org/details/x05-74598_0600_Platform_SDK_April2000_Edition.iso.7z> (a torrent is offered there too).

## Contents

| path | what |
|---|---|
| `D2Net/`, `D2Lang/`, ... | one directory per module: the source, export `.def` files, and `lib/*.def` for the Fog.dll/Storm.dll import libraries |
| `include/` | headers shared by the modules |
| `setup.sh` | makes `toolchain/` from the downloads |
| `build.sh` | compiles and links every module |
| `LICENSE` | MIT License |
| `tools/msvc.sh` | runs `cl`/`lib` under wibo and `link` under Wine, with the linker on a fixed clock so it writes the original timestamps |
| `tools/sdk-include.py` | rebuilds the SDK's header directory from its cabinet files |

`toolchain/`, `downloads/` and `out/` are created by the scripts and are not part of the repository.

## License and disclaimer

The source code and scripts in this folder are released under the MIT License; see [`LICENSE`](LICENSE). The
license covers only this project's own files. It does not cover the Microsoft tools and SDK that `setup.sh`
downloads, nor any Diablo II game files, which remain the property of their respective owners.

Diablo II Hausos is an unofficial, fan-made project. It is not affiliated with, endorsed, sponsored or approved by
Blizzard Entertainment, Inc. Diablo, Diablo II, Blizzard and Blizzard Entertainment are trademarks or registered
trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries. All other trademarks are the property
of their respective owners.
