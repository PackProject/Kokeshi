from argparse import ArgumentParser
from sys import argv
from json import loads
from datetime import datetime
from os import makedirs

from libkiwi_py.stream.file_stream import FileStream, OpenMode, DataMode
from libkiwi_py.stream.stream_base import Endian
from libkiwi_py.message.desc_block import DESCBlock
from libkiwi_py.message.data_block import DATABlock
from libkiwi_py.message.kmsg_block import KMSGBlock


def write_binary(messages: list[str], args):
    """Write specified messages to BKMSG binary"""

    try:
        strm = FileStream(Endian.Big)
        strm.open(args.outfile, OpenMode.Write, DataMode.Binary)
    except OSError:
        print(f"[FATAL] Could not open BKMSG file for writing: {args.outfile}")
        return

    # Header
    header_blk = KMSGBlock()

    # Create blocks
    data_blk = DATABlock(messages)
    desc_blk = DESCBlock.from_data_block(data_blk)

    # Append blocks
    header_blk.append_child(desc_blk)
    header_blk.append_child(data_blk)

    # Write to file
    header_blk.write(strm)

    strm.close()


def write_header(message_keys: list[str], args):
    """Write C++ header file for use with the BKMSG binary"""

    header_path = args.header

    # Autogenerate header path if not specified
    if header_path == None:
        # Split up JSON path
        last_dir_idx = max(args.infile.rfind("/"), args.infile.rfind("\\"))
        file_ext_idx = args.infile.rfind(".")

        file_name = args.infile[last_dir_idx+1:file_ext_idx]
        file_dir = args.infile[:last_dir_idx+1]

        # Ideally, "assets/*" maps to "src/*", so for autogenerating this path,
        # we rely on the KMSG being somewhere inside the assets folder.
        if "assets" not in file_dir:
            print("[FATAL] KMSG file not in assets folder! I do not know where to put the header file, please specify using --header.")
            return

        # Path to header file
        header_dir = file_dir.replace("assets", "src")
        header_path = f"{header_dir}BKMSG_{file_name}.hpp"

    # Create header directory if it doesn't exist
    makedirs(header_dir, exist_ok=True)

    with open(header_path, "w+") as f:
        # Comment from tool
        now = datetime.now()
        f.write("/**\n")
        f.write(
            f" * Auto-generated by make_message.py on {now.strftime('%m/%d/%Y (%H:%M:%S)')}\n")
        f.write(" * Don't edit this unless you know what you are doing,\n")
        f.write(" * as manual changes will be overwritten on the next build.\n")
        f.write(" */\n")

        f.write("\n")

        # Begin header guard
        f.write(f"#ifndef BKMSG_{file_name.upper()}_H\n")
        f.write(f"#define BKMSG_{file_name.upper()}_H\n")

        f.write("\n")

        f.write(f"enum BKMSG_{file_name} {{\n")
        for msg_key in message_keys:
            f.write(f"    {msg_key},\n")
        f.write("};\n")

        f.write("\n")

        # End header guard
        f.write(f"#endif\n")


def convert_json(args):
    """Parse and validate JSON message file, before converting it to binary form"""

    # Attempt to decode JSON data
    try:
        with open(args.infile, "rb") as f:
            json_data = loads(f.read())
    except FileNotFoundError:
        print(f"[FATAL] JSON file could not be opened: {args.infile}")
        return
    except UnicodeDecodeError:
        print(f"[FATAL] JSON data could not be decoded: {args.infile}")
        return

    # Message data
    msg_messages = json_data.get("messages", [])
    if len(msg_messages) == 0:
        print("[FATAL] No messages exist in the JSON file")
        return

    # BKMSG binary
    write_binary(msg_messages.values(), args)
    # Generate C++ header
    write_header(msg_messages.keys(), args)


def main():
    # Parse command-line arguments
    parser = ArgumentParser()
    parser.add_argument("--infile", type=str, required=True,
                        help="KMSG (JSON) file from which to create message binary")
    parser.add_argument("--outfile", type=str, required=True,
                        help="(BKMSG) Path to output message binary")
    parser.add_argument("--header", type=str, required=False,
                        help="Path to C++ header file")
    args = parser.parse_args(argv[1:])

    convert_json(args)


if __name__ == "__main__":
    main()
