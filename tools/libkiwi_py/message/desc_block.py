from libkiwi_py.binary.block.block_base import BlockBase
from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.message.data_block import DATABlock
from libkiwi_py.utility.util import Util


class DESCBlock(BlockBase):
    """Message descriptor block"""

    SIGNATURE = "DESC"

    def __init__(self, offsets: list[int] = []):
        super().__init__(self.SIGNATURE)

        self.add_member(Primitive("u32", "numMsg", value=len(offsets)))
        self.add_member(
            Primitive("u32", "msgOffsets", arr="[]", value=offsets))

    def add_offset(self, offset: int):
        """Add message offset to block"""
        self["numMsg"].value += 1
        self["msgOffsets"].value.append(offset)

    @staticmethod
    def from_data_block(block: DATABlock):
        """Generate DESC block from message data"""
        offsets = []
        offset_now = 0

        for msg in block["pool"]:
            offsets.append(offset_now)
            offset_now += Util.wstr_len(msg, terminator=True)

        return DESCBlock(offsets)
