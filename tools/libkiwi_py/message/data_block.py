from libkiwi_py.binary.types.string import WString
from libkiwi_py.binary.block.block_base import BlockBase


class DATABlock(BlockBase):
    """Message data (pool) block"""

    SIGNATURE = "DATA"

    def __init__(self, messages: list[str] = []):
        super().__init__(self.SIGNATURE)

        self.add_member(WString("pool", arr="[]", value=messages))

    def add_message(self, message: str):
        """Add message to pool"""
        self["pool"].value.append(message)
