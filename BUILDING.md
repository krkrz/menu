# CMake build

Run from the `menu` directory with CMake and Visual Studio 2019 installed:

```powershell
cmake -S . -B build_x86 -G "Visual Studio 16 2019" -A Win32
cmake --build build_x86 --config Release --parallel
ctest --test-dir build_x86 -C Release --output-on-failure

cmake -S . -B build_x64 -G "Visual Studio 16 2019" -A x64
cmake --build build_x64 --config Release --parallel
ctest --test-dir build_x64 -C Release --output-on-failure
```

Outputs: `build_x86/Release/menu.dll` and `build_x64/Release/menu.dll`.
The sibling `tp_stub` sources are required. The DLL uses the static MSVC runtime,
keeps Japanese, Chinese, and English messages in the source, and exports `V2Link` and `V2Unlink`.

The lifetime test exercises repeated native menu allocation/destruction, command
ID recycling, submenu ownership, and locked child-list removal. An application
test should additionally repeat label edits in the motion editor with native
context menus enabled; the standalone test does not run the TJS event loop.

No resource compiler, .rc, .def, or vc2012 directory is required. Win32 uses
source-level linker export aliases; x64 uses the declared C exports directly.
