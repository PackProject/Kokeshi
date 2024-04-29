from libkiwi_py.layout.pan1_block import PaneRes
from libkiwi_py.binary.block.block_base import BlockBase


class PAS1Block(BlockBase):
    """Pane children (START) block"""

    SIGNATURE = "pas1"

    def __init__(self, children=[]):
        super().__init__(self.SIGNATURE)
        self.add_member(PaneRes("panes", arr="[]"))

        for child in children:
            self.append_pane(child)

    def append_pane(self, res):
        """Append child pane"""
        pane = PaneRes("dummy", values=res)
        self["panes"].append(pane)
