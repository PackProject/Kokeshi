from libkiwi_py.binary.block.block_base import BlockBase
from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.binary.types.common import Size


class LYT1Block(BlockBase):
    """Layout block"""

    SIGNATURE = "lyt1"

    def __init__(self, res):
        super().__init__(self.SIGNATURE)

        # Extract from JSON
        self.add_member(Primitive("bool", "centered",
                        value=res.get("centered")))
        self.add_member(Primitive("u8", "padding0", arr="[3]"))
        self.add_member(Size("size", values=res.get("size")))
