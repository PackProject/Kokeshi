#
# Kokeshi clean script
#
# Don't modify this unless you know what you're doing!
#

from shutil import rmtree

#
# Directories
#

ASSETS_DIR = "assets"
BUILD_DIR = "build"


def main():
    # Remove build artifacts
    try:
        rmtree(BUILD_DIR)
    except FileNotFoundError:
        # OK if the directory is already gone
        pass
    except Exception:
        # Something else happened :(
        print("[WARN] Could not delete the build directory.")

    # Remove built assets (TODO)
    pass


if __name__ == "__main__":
    main()
