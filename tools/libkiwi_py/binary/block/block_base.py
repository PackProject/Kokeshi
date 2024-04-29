from libkiwi_py.stream.block_builder import BlockBuilder
from libkiwi_py.stream.stream_base import Endian, StreamBase
from libkiwi_py.binary.types.member import Member


class BlockBase:
    """Base block class"""

    # All blocks are aligned to 32 bytes
    BLOCK_ALIGN = 32

    # Block magic/kind is limited to 4 bytes
    KIND_MAX_LEN = 4

    def __init__(self, kind: str):
        self.kind = kind
        self.builder = BlockBuilder(Endian.Big)
        self.__members = dict()

    def before_build_callback(self):
        """Override to implement custom serialization before the block contents are built"""
        pass

    def after_build_callback(self):
        """Override to implement custom serialization after the block contents are built"""
        pass

    def __getitem__(self, key: str) -> Member:
        """Access block member"""
        assert key in self.__members, f"Member does not exist: {key}"
        return self.__members[key]

    def add_member(self, member: Member):
        """Add member to block"""
        assert member.name not in self.__members, f"Duplicate member declaration: {member.name}"
        self.__members[member.name] = member

    def offset_of(self, key: str):
        """Offset of member in block"""
        # Add up offsets until we hit the desired member
        offset = 0
        offset += 4  # u32 kind
        offset += 4  # u32 size

        for item in self.__members.items():
            # Target member found
            if item[0] == key:
                return offset
            # Keep going
            offset += item[1].byte_size()

        # Member is not in block
        return -1

    def byte_size(self) -> int:
        """Size of block in bytes"""
        bsize = 0
        bsize += 4  # u32 kind
        bsize += 4  # u32 size
        bsize += self.builder.buffer_size()  # Block data
        return bsize

    def aligned_size(self) -> int:
        """Size of block in bytes, aligned to BLOCK_ALIGN"""
        bsize = self.byte_size()
        align = self.BLOCK_ALIGN - (bsize % self.BLOCK_ALIGN)
        return bsize + align

    def write(self, strm: StreamBase):
        """Write block builder to stream"""

        self.before_build_callback()

        # Serialize to block builder
        for m in self.__members.values():
            m.write(self.builder)

        self.after_build_callback()

        # Block kind
        strm.write_string(self.kind, maxlen=self.KIND_MAX_LEN, terminate=False)
        # Block size
        strm.write_u32(self.aligned_size())
        # Block data
        strm.write(self.builder.buffer_data())
        # Block padding
        strm.write_padding(self.aligned_size() - self.byte_size())
