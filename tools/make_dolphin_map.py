from argparse import ArgumentParser
from sys import argv


def to_hex(x):
    return hex(x)[2:]


def convert(args):
    with open(args.infile, "r") as f:
        rel_symbols = f.readlines()

    if rel_symbols[0] != "Kamek Binary Map\n":
        print(f"[FATAL] Input file is not a Kamek map file: {args.infile}")
        return

    with open(args.outfile, "w+") as f:
        f.write(".text section layout\n")

        # Kamek Binary Map
        #   Offset   Size   Name
        for sym in rel_symbols[2:]:
            tokens = sym.split()  # offset, size, name

            offset = int(tokens[0], base=16)
            size = int(tokens[1], base=16)
            name = tokens[2]
            rel_base = int(args.base, base=0)

            # Resolve relocation
            addr = rel_base + offset
            f.write(f"{to_hex(addr)} {to_hex(size)} {to_hex(addr)} 0 {name}\n")


def main():
    parser = ArgumentParser()
    parser.add_argument("--infile", type=str, required=True,
                        help="Relocatable map file (Kamek)")
    parser.add_argument("--outfile", type=str, required=True,
                        help="Static map file (Dolphin)")
    parser.add_argument("--base", type=str, required=True,
                        help="Module base address (.text section)")

    args = parser.parse_args(argv[1:])
    convert(args)


if __name__ == "__main__":
    main()
