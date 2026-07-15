# Scrabble.c

A cross-platform C implementation of Scrabble of the popular game of Scrabble.

## Build

See [DevEnvironment.md](DevEnvironment.md) for full setup details.

**Linux / macOS**
```bash
cd build
chmod +x premake5
./premake5 gmake
cd ..
make
./bin/Debug/Scrabble
```

**Windows**
Install mingw64 from [here](https://github.com/skeeto/w64devkit/releases) and add the `.\mingw64\bin` directory to your `PATH` in your environment variables. Then in `cmd`, run the following:
```cmd
    build-MinGW-W64.bat
    make
```

## Features
- Intuitive SFx and BGM
- Full Standard Scrabbble Game logic with Luxury tiles and more.
- Network Play mode along with Local Turn-by-Turn.

## Status
**Working:** menu/settings, tile bag, player scores & racks
**Not yet implemented:** board interaction, word validation, scoring multipliers, turns, multiplayer (local & LAN), save/load, AI opponent, audio

## Contributing

PRs are welcomed with open arms. Fork, make your changes, open a PR.