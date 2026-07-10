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
* **For the first time**
    1. Clone this repo 
    ```bash
        git clone https://github.com/sonothamin/Scrabble.c
    ```
    2. Install `make`, `gcc`, `g++`, `gdb`, etc.
    ```bash
        sudo apt update
        sudo apt install build-essential
    ```
    3. Go to `[Project Root]/build`, run `premake5` and come back to root directory
    ```bash
        cd ./Scrabble.c/build
        chmod +x premake5
        ./premake5 gmake
        cd ..
    ```
    You should be good to go !!

* **From now on, run the following to compile your code:**

    1. Run `make` to compile the code to an executable binary
    ```bash
        make
    ```
    2. Execute the resulting binary
    ```bash
        ./bin/Debug/Scrabble
    ```

## Windows build

* Using wMinGW64 **(Sonoth Recommended)**
    1. Cleanly uninstall any and all pre-existing MinGW from your system
    2. Install w64devkit from here https://github.com/skeeto/w64devkit/releases
    3. Add the `[Path of the extracted w64devkit folder]/bin/` as `Path` in your Windows environment variables.
    4. Open project folder and Double click `build-MinGW-W64.bat` in File Explorer
    5. You should be good to go!! 
    
        For all susequent build, run `make` in the root of this repo folder. The executable `.exe` will be at `/bin/Debug/Scrabble.exe`.

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