# Kokeshi
Kokeshi is a framework for injecting custom code into Pack Project titles. It is powered by the [Kamek](https://github.com/Treeki/Kamek) tool which loads and dynamically links code at runtime.

## Repository Map
```
├── 🎨 assets: Graphics, audio, and other data
├── 📜 base: Base game DOLs and symbol maps
├── 🚧 build: Build process output
├── 📜 include: Header files for interfacing with the game(s)
├── 📚 lib: External library code
│      └── 🥝 libkiwi: Utility library for working with Pack Project games
├── 🔄 loader: Statically-linked code (Kamek loader)
├── 💿 src: Dynamically-linked code (Kamek module)
└── 🔨 tools: Build tools (Kamek linker, CodeWarrior compiler, etc.)
```

## Supported Games
Kokeshi supports the following games:
| Name              | Revision      | `main.dol` SHA1                            |
| ----------------- | ------------- | ------------------------------------------ |
| Wii Sports        | NTSC-U, Rev 1 | `8bb422971b88b5551a37de98db69557df7b46637` |
| Wii Play          | NTSC-U, Rev 1 | `0da5e7e51135219f580ad011d1b635bc83569bb9` |
| Wii Sports Resort | NTSC-U, Any*  | `e3e22a9de62f6e11ded52e9a7e6933892963b219` |

**Revisions of Wii Sports Resort only include minor asset changes. The DOL remains unchanged.*

Support for Wii Music and Wii Fit, as well as other revisions of supported games are currently out of scope of this project. This is unfortunately due to the lack of debugging symbols in these games.

## Build Instructions

### Setup

#### Tools
1. Install GNU `make`. If you have DevKitPRO or Windows Subsystem for Linux you may already have the tool installed.
2. Download the latest release of [the PackProject fork of Kamek](https://github.com/PackProject/Kamek).
   1. Install the contents of the zip file to the `tools/` directory.
3. Download the Wii v1.7 version of [the CodeWarrior compiler](https://files.decomp.dev/compilers_20230715.zip).
   1. Install the contents of the "1.7" folder to the `tools/` directory.
   
#### Filesystem
1. For each game that you want to build for, extract its filesystem using Dolphin Emulator.
   1. This can be done by right-clicking the game and selecting `Properties > Filesystem > Disc > Extract Entire Disc...`
   2. When prompted by Dolphin, select the directory corresponding to the game:
        | Name              | Path            |
        | ----------------- | --------------- |
        | Wii Sports        | `romfs/sports`  |
        | Wii Play          | `romfs/play`    |
        | Wii Sports Resort | `romfs/sports2` |
    3. If done correctly, this will create a `DATA` folder in the corresponding `romfs` game folder. 

2. For each game that you want to build for, copy `DATA/sys/main.dol` from its `romfs` folder to the `base/` directory:
    | Name              | Path                       |
    | ----------------- | -------------------------- |
    | Wii Sports        | `base/baserom_sports.dol`  |
    | Wii Play          | `base/baserom_play.dol`    |
    | Wii Sports Resort | `base/baserom_sports2.dol` |

#### Makefile
The `Makefile` provides various options for your project. You can choose which games to build your mod for by modifying the `PACK` variable. You can even write multiple names to build for multiple games!

### Running
To build your mod, simply run `make` from the project root directory. A `make clean` target is also provided to delete the contents of the `build/` directory.

The default build target is the debug configuration. To disable debug features such as logging/assertions, run `make` with `NDEBUG=1`.

## Troubleshooting

### `Baserom is missing or could not be opened`
One or more of the base DOL files are either in the wrong place or are currently open by another program. See the "Build Instructions" section if you are unusure where the files should go.

### `Baserom does not have the correct hash`
One or more of the base DOL files have the wrong SHA1 hash. This means that they are likely an unsupported game revision. See the "Supported Games" section if you are unusure which revisions are supported.

### `Error while compiling Kamek loader` / `Error while linking Kamek loader`
There was a problem building the Kamek loader. If you did not touch the loader code (in the `loader/` directory), this may be a problem with Kokeshi.

It is also possible that you have the wrong version of CodeWarrior/Kamek or are missing one of these tools entirely.

### `Error while compiling your module`
An error occurred in the compiler. This is likely a result of your code, but could be a problem with Kokeshi. Make sure to read the compiler error to understand exactly where the problem is.

### `Error while linking your module`
An error occurred in the Kamek linker. Based on the exception it spits out, you can determine what is happening:
#### `System.InvalidOperationException: duplicate commands for address`
You have more than one Kamek hook placed at the same address. This is not possible for the linker as they would overwrite each other's contents.

Kokeshi does not install many hooks itself, but it is possible you may be conflicting with `libkiwi`. Please search the repository for the address you are trying to hook, and see if there are conflicts.
#### `System.IO.InvalidDataException: undefined symbol`
You are referencing a symbol in the base game but the linker cannot resolve its address. This means it is missing from the symbol map(s) in the `base/` directory.

Kamek will give you the mangled symbol name (i.e. `calc__10RPSysSceneFv`), which can be placed into the symbol map corresponding to your game with the following syntax: `{name}=0x{address}`.

**This is very common because these symbol maps are created by hand and manually updated as more of the code is documented and understood.**

If you add many symbols for your mod please consider making a pull request to Kokeshi to add these symbols for everyone else. :)

### `RomFS is not setup correctly`
One or more of the base game filesystems were not extracted correctly. In the `romfs` folder for your game(s), the following directories must exist:
```
├── DATA: Disc partition root
│    └── sys: Disc header, DOL executable
│    └── files: Disc filesystem
```

See the "Build Instructions" section if you are unusure how to extract the game(s) correctly.
