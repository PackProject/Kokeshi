from libkiwi_py.stream.stream_base import StreamBase


class BlockBuilder(StreamBase):
    """Block buffer builder"""

    def __init__(self, endian: int):
        super().__init__(endian)
        self.__data = bytearray()

    def read(self, size: int) -> bytearray:
        """Read bytes from stream"""
        raise NotImplementedError("BlockBuilder does not support reading")

    def write(self, data: bytearray):
        """Write bytes to stream"""
        self.__data += data

    def eof(self) -> bool:
        """Check whether the stream has hit the end-of-file"""
        return False

    def seek(self, origin: int, offset: int):
        """Seek the stream position"""
        raise NotImplementedError("BlockBuilder does not support seeking")

    def open(self, data: bytearray = bytearray()):
        """Open stream (no buffer specified = create one)"""
        self.__data = data

    def buffer_data(self) -> bytearray:
        """Access buffer data"""
        return self.__data

    def buffer_size(self) -> int:
        """Access buffer size"""
        return len(self.__data)
