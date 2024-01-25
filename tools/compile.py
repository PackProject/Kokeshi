#
# Kokeshi code build script
#
# Don't modify this unless you know what you're doing!
#

from argparse import ArgumentParser
from os import walk, makedirs
from os.path import join, exists, split
from shutil import copyfile, copytree
from subprocess import run, PIPE
from hashlib import sha1

#
# Configuration
#

# DOL address where the Kamek loader will be statitcally linked.
# Change it if you need this memory area for whatever reason.
LOADER_ADDR = 0x80001900

#
# Compiler flags
#

# Flags applied to all source files
CFLAGS_COMMON = " ".join([
    "-proc gekko",          # Gekko processor
    "-i .",                 # Root include directory
    "-I-",
    "-Cpp_exceptions off",  # Disable C++ exceptions
    "-enum int",            # Force 4-byte enumerations
    "-O4,s",                # Max optimization level, focus on small code size
    "-use_lmw_stmw on",     # Use lmw/stmw for stack frames to save code size
    "-fp fmadd",            # Use compact floating-point instructions to save code size
    "-rostr",               # Place strings in rodata
    "-RTTI off",            # Disable RTTI to save module size
    "-sdata 0",             # We can't use small data sections in relocatable code
    "-sdata2 0",            # We can't use small data sections in relocatable code
])

# Flags applied for debug target
CFLAGS_DEBUG = " ".join([
])

# Flags applied for release target
CFLAGS_RELEASE = " ".join([
    "-DNDEBUG"
])

#
# Directories
#

BUILD_DIR = "build"
BASE_DIR = "base"
ROMFS_DIR = "romfs"
MODULES_DIR = "modules"
INCLUDE_DIR = "include"
LIBRARY_DIR = "lib"
LOADER_DIR = "loader"
SRC_DIR = "src"
TOOLS_DIR = "tools"

#
# Tools
#

CC = f"{TOOLS_DIR}\\mwcceppc.exe"
KAMEK = f"{TOOLS_DIR}\\Kamek.exe"

#
# File extensions
#

SRC_EXTENSIONS = [".c", ".cpp", ".cc", ".cxx"]
INCLUDE_EXTENSIONS = [".h", ".hpp", ".hh", ".hxx"]

#
# Known good file hashes
#

GOOD_HASHES = {
    "sports": [
        "8bb422971b88b5551a37de98db69557df7b46637"  # Wii Sports (US, Rev 1)
    ],

    "play": [
        "0da5e7e51135219f580ad011d1b635bc83569bb9"  # Wii Play (US, Rev 1)
    ],
}

#
# Functions
#


def main() -> None:
    """Main function
    """

    parser = ArgumentParser(prog="Kokeshi")

    parser.add_argument("--game", type=str, required=True,
                        choices=["sports", "play", "resort"], help="Game name")

    parser.add_argument("--target", type=str, required=False,
                        default="debug", choices=["debug", "release"], help="Build target")

    parser.add_argument("--cflags", type=str, required=False,
                        help="Register a custom set of compiler flags")

    parser.add_argument("--define", action='append',
                        required=False, help="Register a custom preprocessor definition of the form \"KEY,VALUE\"")

    args = parser.parse_args()
    build(args)


def build(args) -> None:
    """Attempt to build the mod

    Args:
        args: Parsed command-line arguments
    """

    print("[INFO] Checking baserom...")
    if not baserom_ok(args):
        return

    print("[INFO] Building Kamek loader...")
    if not build_loader(args):
        return

    print(f"[INFO] Building module...")
    if not build_module(args):
        return

    print(f"[INFO] Installing to romfs...")
    if not install_romfs(args):
        return

    print(f"[INFO] Success!")


def search_files(root: str, extensions: list[str]) -> list[str]:
    """Search directory recursively for files with the specified extensions

    Args:
        root (str): Root directory
        extensions (list[str]): Allowed file extensions

    Returns:
        list[str]: Search results
    """

    found = []

    for dirpath, _, filenames in walk(root):
        for name in filenames:
            # Isolate file extension
            ext = name[name.rfind("."):]

            # Check whitelist
            if ext in extensions:
                # Absolute path
                found.append(join(dirpath, name))

    return found


def search_dirs(root: str, recursive: bool) -> list[str]:
    """Search directory recursively for subdirectories

    Args:
        root (str): Root directory

    Returns:
        list[str]: Search results
    """
    found = [root]

    for dirpath, dirnames, _ in walk(root):
        # Absolute path
        found += [join(dirpath, x) for x in dirnames]

        if not recursive:
            break

    return found


def src_to_obj(path: str) -> str:
    """Convert source file path to object file path

    Args:
        path (str): Source file path

    Returns:
        str: Object file path
    """

    # Sort extensions by length to ensure greedy matching
    for ext in sorted(SRC_EXTENSIONS, key=lambda x: len(x)):
        # Replace with object extension, and also put it in the build directory
        if ext in path:
            return f"{BUILD_DIR}/{path.removesuffix(ext)}.o"

    return path


def compile(path: str, flags: str) -> bool:
    """Attempt to compile source file

    Args:
        path (str): Source file path
        flags (str): Compiler flags

    Returns:
        bool: Success
    """

    # Object file that will be created
    obj = src_to_obj(path)
    # Make directories if we need to for this object
    makedirs(split(obj)[0], exist_ok=True)

    # Shell command
    cmd = f"{CC} {flags} -c -o {obj} {path}"

    # Run CW compiler
    result = run(cmd, shell=True, stdout=PIPE,
                 stderr=PIPE, universal_newlines=True)

    # Check for compiler error
    if result.returncode != 0:
        print(result.returncode, result.stdout, result.stderr)

    return result.returncode == 0


def link(files: list[str], flags: str) -> bool:
    """Attempt to link object files

    Args:
        files (list[str]): Object files
        ldflags (str): Linker flags

    Returns:
        bool: Success
    """

    # Shell command
    cmd = f"{KAMEK} {' '.join(files)} {flags}"

    # Run Kamek linker
    result = run(cmd, shell=True, stdout=PIPE,
                 stderr=PIPE, universal_newlines=True)

    # Check for linker error
    if result.returncode != 0:
        print(result.returncode, result.stdout, result.stderr)

    return result.returncode == 0


def baserom_ok(args) -> bool:
    """Check for the existence & correctness of the base game executable

    Args:
        args: Parsed command-line arguments

    Returns:
        bool: Success
    """

    # Where the base DOL should be
    rom_path = f"{BASE_DIR}/baserom_{args.game}.dol"

    # Read executable contents
    try:
        f = open(rom_path, "rb")
        dol = f.read()
    except OSError:
        print(f"[FATAL] Baserom is missing or could not be opened: {rom_path}")
        return False

    # Verify hash
    ctx = sha1()
    ctx.update(dol)
    if ctx.hexdigest() not in GOOD_HASHES[args.game]:
        print(f"[FATAL] Baserom does not have the correct hash: {rom_path}")
        return False

    return True


def build_loader(args) -> bool:
    """Compile & link Kamek loader program

    Args:
        args: Parsed command-line arguments

    Returns:
        bool: Success
    """

    #
    # Compilation step
    #

    # Identify loader files
    srcs = search_files(LOADER_DIR, SRC_EXTENSIONS)
    objs = [src_to_obj(f) for f in srcs]

    inc_dirs = search_dirs(SRC_DIR, recursive=False) + \
        search_dirs(INCLUDE_DIR, recursive=False)
    inc_dirs.append(LOADER_DIR)

    # Loader compiler flags
    cflags = ' '.join([
        # Common flags
        CFLAGS_COMMON,
        # Pack definition
        f"-DPACK_{args.game.upper()}",
        # Build target flags (release/debug)
        ' '.join(CFLAGS_DEBUG if args.target == "debug" else CFLAGS_RELEASE),
        # User flags
        args.cflags,
        # Include flags
        ' '.join([f"-ir {x}" for x in inc_dirs])
    ])

    # Compile source files
    for f in srcs:
        if not compile(f, cflags):
            print("[FATAL] Error while compiling Kamek loader.")
            return False

    #
    # Link step
    #

    # Kamek loader flags
    ldflags = " ".join([
        f"-static={hex(LOADER_ADDR)}",
        f"-input-dol={BASE_DIR}/baserom_{args.game}.dol",
        f"-output-dol={BUILD_DIR}/main_{args.game}.dol",
        f"-output-kamek={BUILD_DIR}/{LOADER_DIR}/Loader_{args.game}.bin",
        f"-output-map={BUILD_DIR}/{LOADER_DIR}/Loader_{args.game}.map",
        f"-externals={BASE_DIR}/symbols_{args.game}.txt"
    ])

    if not link(objs, ldflags):
        print("[FATAL] Error while linking Kamek loader.")
        return False

    return True


def build_module(args) -> bool:
    """Compile & link module program

    Args:
        args: Parsed command-line arguments

    Returns:
        bool: Success
    """

    #
    # Compilation step
    #

    # Identify module files
    srcs = search_files(SRC_DIR, SRC_EXTENSIONS) + \
        search_files(LIBRARY_DIR, SRC_EXTENSIONS)
    objs = [src_to_obj(f) for f in srcs]

    inc_dirs = search_dirs(SRC_DIR, recursive=False) + search_dirs(
        INCLUDE_DIR, recursive=False) + search_dirs(LIBRARY_DIR, recursive=False)
    inc_dirs.append(LOADER_DIR)

    # Module compiler flags
    cflags = ' '.join([
        # Common flags
        CFLAGS_COMMON,
        # Pack definition
        f"-DPACK_{args.game.upper()}",
        # Build target flags (release/debug)
        ' '.join(CFLAGS_DEBUG if args.target == "debug" else CFLAGS_RELEASE),
        # User flags
        args.cflags,
        # Include flags
        ' '.join([f"-ir {x}" for x in inc_dirs])
    ])

    # Compile source files
    for f in srcs:
        if not compile(f, cflags):
            print("[FATAL] Error while compiling your module.")
            return False

    #
    # Link step
    #

    # Make modules directory if we need to
    makedirs(f"{BUILD_DIR}/{MODULES_DIR}", exist_ok=True)

    # Kamek module flags
    ldflags = " ".join([
        f"-output-kamek={BUILD_DIR}/{MODULES_DIR}/m_{args.game}.bin",
        f"-output-map={BUILD_DIR}/{MODULES_DIR}/m_{args.game}.map",
        f"-externals={BASE_DIR}/symbols_{args.game}.txt"
    ])

    if not link(objs, ldflags):
        print("[FATAL] Error while linking your module.")
        return False

    return True


def install_romfs(args) -> bool:
    """Move everything into its appropriate romfs location

    Args:
        args: Parsed command-line arguments

    Returns:
        bool: Success
    """
    # Assume Dolphin format (DATA -> disc,files,sys)
    disc_root = f"{ROMFS_DIR}/{args.game}"  # Extracted disc
    partition_root = f"{disc_root}/DATA"    # DATA partition (RomFS)

    # Folders that must exist in this disc dump
    must_exist = [
        disc_root,
        partition_root,
        f"{partition_root}/sys",    # DOL goes here
        f"{partition_root}/files",  # Module goes here (well, files/modules...)
    ]

    # Check RomFS structure
    for path in must_exist:
        if not exists(path):
            print(f"""[FATAL] RomFS is not setup correctly. Missing the following directory: {partition_root}
            Please use Dolphin Emulator's \"Extract Game Disc...\" option, and extract the contents into the following folder:
            {disc_root}""")
            return False

    # Install loader (sys/main.dol patch)
    copyfile(f"{BUILD_DIR}/main_{args.game}.dol",
             f"{partition_root}/sys/main.dol")

    # Install module & mapfile
    copytree(f"{BUILD_DIR}/{MODULES_DIR}",
             f"{partition_root}/files/{MODULES_DIR}", dirs_exist_ok=True)

    return True


#
# Top-level code
#
if __name__ == "__main__":
    main()
