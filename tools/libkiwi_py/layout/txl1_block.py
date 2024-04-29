from libkiwi_py.binary.block.block_base import BlockBase
from libkiwi_py.binary.types.structure import Structure
from libkiwi_py.binary.types.string import String
from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.stream.stream_base import StreamBase
from libkiwi_py.utility.util import Util


class TextureDesc(Structure):
    """Texture descriptor structure"""

    MEMBERS = [
        Primitive("u32", "offset"),
        Primitive("u8", "padding0", arr="[4]")
    ]


class TXL1Block(BlockBase):
    """Texture list block"""

    SIGNATURE = "txl1"

    def __init__(self, textures: list[str] = []):
        super().__init__(self.SIGNATURE)
        self.__pool_size = 0

        self.add_member(Primitive("u16", "numEntries", value=len(textures)))
        self.add_member(Primitive("u8", "padding0", arr="[2]"))
        self.add_member(TextureDesc("texDescs", arr="[]"))
        self.add_member(String("texNames", arr="[]"))

        # Add textures to pool
        for t in textures:
            self.add_texture(t)

    def add_texture(self, texture: str):
        """Add texture to pool"""
        # Add texture name to pool
        self["texNames"].value.append(texture)

        # Add new texture descriptor
        desc = TextureDesc("dummy", values={"offset": self.__pool_size})
        self["texDescs"].append(desc)

        # Increase texture count
        self["numEntries"].value += 1

        # Update pool size
        self.__pool_size += Util.str_len(texture, terminator=True)

    def before_build_callback(self):
        """Custom block behavior before block contents are serialized"""
        # Finalize string pool offsets
        for desc in self["texDescs"]:
            # Convert pool-relative offset to section-relative offset
            desc["offset"].value += self.offset_of("texNames")
