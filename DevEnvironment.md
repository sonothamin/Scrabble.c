# Development Environment Setup for Scrabble.c

This repository contains a raylib-based C implementation of Scrabble. The project already includes the build files needed to compile it on Linux, Windows, and macOS.

## Requirements

You will need:
- A C compiler such as GCC or Clang
- make
- Git
- VS Code (optional, but recommended)

## Project structure

- src/ contains the game source code
- include/ contains shared headers
- resources/ contains game assets such as images and data files
- bin/ contains the built executables
- build/ contains the Premake configuration and bundled raylib setup

## Linux build

From the project root:

```bash
make
```

To build a specific configuration:

```bash
make config=debug_x64
```

The executable is written to:

```bash
bin/Debug/Scrabble
```

Run it with:

```bash
./bin/Debug/Scrabble
```

## Windows Build
There are two compiler toolchains available for windows, MinGW-W64 (a free compiler using GCC), and Microsoft Visual Studio
### Using MinGW-W64
* Rename the folder to your game name
* Double click the `build-MinGW-W64.bat` file
* CD into the folder in your terminal
  * if you are using the W64devkit and have not added it to your system path environment variable, you must use the W64devkit.exe terminal, not CMD.exe
  * If you want to use cmd.exe or any other terminal, please make sure that gcc/mingw-W64 is in your path environment variable.
* run `make`
* You are good to go

### Note on MinGW-64 versions
Make sure you have a modern version of **MinGW-W64** (not **mingw**).
The best place to get it is from the W64devkit from
https://github.com/skeeto/w64devkit/releases

or the version installed with the raylib installer

#### If you have installed raylib from the installer
Make sure you have added the path

`C:\raylib\w64devkit\bin`

To your path environment variable so that the compiler that came with raylib can be found.

DO NOT INSTALL ANOTHER MinGW-W64 from another source such as msys2, you don't need it.


## File Structure

Game assets should be placed in:`resources/`

Include headers should be in:`include/`

Logic C files should reside in: `src/`

The project already includes logic for locating that directory at runtime.

## Recommended workflow

1. Open the repository in VS Code
2. Edit source files in `/src/` or `/include/`
3. Rebuild with `make`
4. The compiled executable binary will be at `bin/Debug/` or `bin/Release/`

## Future development notes

If new external libraries are introduced, they should be added to build/premake5.lua.
