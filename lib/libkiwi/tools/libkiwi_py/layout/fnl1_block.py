from libkiwi_py.binary.block.block_base import BlockBase
from libkiwi_py.binary.types.structure import Structure
from libkiwi_py.binary.types.string import String
from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.stream.stream_base import StreamBase
from libkiwi_py.utility.util import Util


class FontDesc(Structure):
    """Font descriptor structure"""

    MEMBERS = [
        Primitive("u32", "offset"),
        Primitive("u8", "padding0", arr="[4]")
    ]


class FNL1Block(BlockBase):
    """Font name list block"""

    SIGNATURE = "fnl1"

    def __init__(self, fonts: list[str] = []):
        super().__init__(self.SIGNATURE)
        self.__pool_size = 0

        self.add_member(Primitive("u16", "numEntries", value=len(fonts)))
        self.add_member(Primitive("u8", "padding0", arr="[2]"))
        self.add_member(FontDesc("fontDescs", arr="[]"))
        self.add_member(String("fontNames", arr="[]"))

        # Add fonts to pool
        for f in fonts:
            self.add_font(f)

    def add_font(self, font: str):
        """Add font to name list"""
        # Add font name to pool
        self["fontNames"].value.append(font)

        # Add new font descriptor
        desc = FontDesc("dummy", values={"offset": self.__pool_size})
        self["fontDescs"].append(desc)

        # Increase font count
        self["numEntries"].value += 1

        # Update pool size
        self.__pool_size += Util.str_len(font, terminator=True)

    def before_build_callback(self):
        """Custom block behavior before block contents are serialized"""
        # Finalize string pool offsets
        for desc in self["fontDescs"]:
            # Convert pool-relative offset to section-relative offset
            desc["offset"].value += self.offset_of("fontNames")
