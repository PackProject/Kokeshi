#
# Kokeshi asset build script
#
# Don't modify this unless you know what you're doing!
#

from shutil import copytree
from compile import ROMFS_DIR
from argparse import ArgumentParser
from os.path import exists

#
# Directories
#

ASSETS_DIR = "assets"


def main() -> None:
    """Main function
    """
    parser = ArgumentParser(prog="Kokeshi")

    parser.add_argument("--game", type=str, required=True,
                        choices=["sports", "play", "resort"], help="Game name")

    parser.add_argument("--ci", type=int, required=False, default=0,
                        help="Use for GitHub CI to avoid the ROM.")

    args = parser.parse_args()

    if not build(args):
        exit(1)

    print("[INFO] Success!")


def build(args) -> bool:
    """Attempt to build the mod

    Args:
        args: Parsed command-line arguments
    """

    if not args.ci:
        print("[INFO] Installing assets...")
        if not install_assets(args):
            return False

    return True


def install_assets(args) -> bool:
    """Move assets into the appropriate romfs location

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

    try:
        # Install assets
        copytree(f"{ASSETS_DIR}/{args.game}",
                 f"{partition_root}/files", dirs_exist_ok=True)
    except Exception as err:
        print("[FATAL] An error occurred while copying assets:")
        print(err)
        return False

    return True


if __name__ == "__main__":
    main()
