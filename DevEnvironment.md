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

## Windows build

* If you are using MinGW-W64 (with VSCode/CodeBlocks/Notepad++ etc), for the first time, run:

    ```bat
    build-MinGW-W64.bat
    make
    ```
    And just run ```make``` for all further builds.


* If you are using Visual Studio, run `build-VisualStudio2022.bat` or `build-VisualStudio2026.bat`. Then open the generated solution file in Visual Studio.

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