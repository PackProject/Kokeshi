from libkiwi_py.binary.block.block_base import BlockBase


class PAE1Block(BlockBase):
    """Pane children (END) block"""

    SIGNATURE = "pae1"

    def __init__(self, children=[]):
        super().__init__(self.SIGNATURE)
