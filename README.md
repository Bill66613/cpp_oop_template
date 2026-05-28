# cpp_oop_template

Small C++ OOP template project with a top-level executable and three libraries:

- `helper`: utility helpers
- `animal`: sample shared library
- `sqlite`: SQLite wrapper resolved through CMake package discovery

## What changed

The repo is now structured around explicit build flows instead of hardcoded host paths:

- Native Windows builds can use the checked-in `vcpkg_installed/x64-windows` tree.
- Native Linux builds can use system packages or a matching `vcpkg_installed/<triplet>` tree.
- Cross builds are driven by CMake presets plus toolchain files under `cmake/toolchains/`.
- Machine-specific paths belong in `CMakeUserPresets.json`, not the shared project files.

## Presets

Run `cmake --list-presets` to see the available configure and build presets.

Important configure presets:

- `default`: current Windows-friendly default using the repo-local dependency tree
- `native-windows-local`: native Windows build without requiring the vcpkg toolchain file
- `native-windows-vcpkg`: native Windows build through manifest-mode vcpkg
- `native-linux-local-debug`: native Linux debug build
- `native-linux-vcpkg-debug`: native Linux debug build with manifest-mode vcpkg
- `mingw-w64-vcpkg-release`: cross build to Windows through MinGW-w64 and vcpkg chainloading
- `linux-aarch64-vcpkg-release`: cross build to Linux aarch64 through GCC sysroot chainloading and vcpkg

## Native build examples

Windows using the repo-local dependency tree:

```powershell
cmake --preset default
cmake --build --preset default
```

Windows using vcpkg manifest mode:

```powershell
$env:VCPKG_ROOT = 'D:/Program Files/vcpkg'
cmake --preset native-windows-vcpkg
cmake --build --preset native-windows-vcpkg-debug
```

Linux using system packages or a local package tree:

```bash
cmake --preset native-linux-local-debug
cmake --build --preset native-linux-local-debug
```

## Cross-compilation

### MinGW-w64 to Windows

Required environment:

- `VCPKG_ROOT`: vcpkg installation path
- `MINGW_W64_ROOT`: MinGW-w64 toolchain root containing `bin/x86_64-w64-mingw32-g++.exe`

Example:

```powershell
$env:VCPKG_ROOT = 'D:/Program Files/vcpkg'
$env:MINGW_W64_ROOT = 'C:/toolchains/mingw64'
cmake --preset mingw-w64-vcpkg-release
cmake --build --preset mingw-w64-vcpkg-release
```

### GCC sysroot to Linux aarch64

Required environment:

- `VCPKG_ROOT`: vcpkg installation path
- `AARCH64_GNU_ROOT`: cross toolchain root containing `bin/aarch64-linux-gnu-g++`
- `AARCH64_SYSROOT`: target sysroot path

Example:

```bash
export VCPKG_ROOT=/opt/vcpkg
export AARCH64_GNU_ROOT=/opt/toolchains/aarch64-linux-gnu
export AARCH64_SYSROOT=/opt/sysroots/aarch64-linux-gnu
cmake --preset linux-aarch64-vcpkg-release
cmake --build --preset linux-aarch64-vcpkg-release
```

## Notes

- The `sqlite` library first tries `unofficial-sqlite3` config packages, then falls back to `find_package(SQLite3)`.
- If you install dependencies to a custom prefix or sysroot, add that location to `CMAKE_PREFIX_PATH`.
- `CMakeUserPresets.json` is intended for local overrides and should stay untracked in normal use.
