from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.binary.types.structure import Structure
from libkiwi_py.binary.types.common import VEC3, VEC2, Size
from libkiwi_py.binary.types.string import String
from libkiwi_py.binary.block.block_base import BlockBase


class PaneRes(Structure):
    """Pane structure"""

    MAX_NAME_LEN = 16
    MAX_USERDATA_LEN = 8

    MEMBERS = [
        Primitive("u8", "flags"),
        Primitive("u8", "base_position"),
        Primitive("u8", "alpha"),
        Primitive("u8", "padding0"),
        String("name", c_style=False, maxlen=MAX_NAME_LEN, pad=True),
        String("user_data", c_style=False, maxlen=MAX_USERDATA_LEN, pad=True),
        VEC3("translate"),
        VEC3("rotate"),
        VEC2("scale"),
        Size("size")
    ]


class PAN1Block(BlockBase):
    """Pane block"""

    SIGNATURE = "pan1"

    def __init__(self, res):
        super().__init__(self.SIGNATURE)
        self.children = []

        assert "name" in res, "Pane is missing name"
        self.add_member(PaneRes("pane", values=res))
