from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.binary.types.common import Size
from libkiwi_py.binary.types.string import WString
from libkiwi_py.binary.types.gx import GXColor
from libkiwi_py.layout.pan1_block import PAN1Block
from libkiwi_py.layout.mat1_block import MAT1Block
from libkiwi_py.layout.fnl1_block import FNL1Block


class TXT1Block(PAN1Block):
    """Pane block"""

    SIGNATURE = "txt1"

    def __init__(self, res, mats: MAT1Block, fonts: FNL1Block):
        super().__init__(res)
        self.kind = self.SIGNATURE

        assert "name" in res, "Text box is missing name"

        self.add_member(Primitive("u16", "text_buf_size"))
        self.add_member(Primitive("u16", "text_str_size"))
        # TODO: Read name from JSON, convert to index using MAT1
        self.add_member(Primitive("u16", "material_index"))
        # TODO: Read name from JSON, convert to index using FNL1
        self.add_member(Primitive("u16", "font_index"))
        # TODO: This is an enum
        self.add_member(Primitive("u8", "text_position"))
        # TODO: This is an enum
        self.add_member(Primitive("u8", "text_alignment"))
        self.add_member(Primitive("u8[2]", "padding0"))
        self.add_member(Primitive("u16", "text_str_offset"))

        self.add_member(GXColor("top_color", values=res.get("top_color")))
        self.add_member(GXColor("bot_color", values=res.get("bot_color")))

        self.add_member(Size("font_size"), res.get("font_size"))
        self.add_member(Primitive("f32", "char_space"), res.get("char_space"))
        self.add_member(Primitive("f32", "line_space"), res.get("line_space"))

        self.add_member(WString("text", value=res.get("text"), c_style=True))
