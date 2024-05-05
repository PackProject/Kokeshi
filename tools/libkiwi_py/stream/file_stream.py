from libkiwi_py.stream.stream_base import StreamBase, SeekOrigin


class OpenMode:
    Read, Write, Max = range(3)


class DataMode:
    Text, Binary, Max = range(3)


class FileStream(StreamBase):
    """File stream"""

    def __init__(self, endian: int):
        super().__init__(endian)
        self._path = None
        self._open_mode = None
        self._data_mode = None
        self._file = None

    def read(self, size: int) -> bytes:
        """Read bytes from stream"""
        assert not self.eof(), "End-of-file while reading"
        assert self._open_mode == OpenMode.Read, "Not for this openmode"

        # No file open
        if self._file == None:
            return bytes()

        result = self._file.read(size)
        assert len(result) == size, "End-of-file while reading"

        return result

    def write(self, data: bytes):
        """Write bytes to stream"""
        assert self._open_mode == OpenMode.Write, "Not for this openmode"

        # No file open
        if self._file == None:
            return

        self._file.write(data)

    def eof(self) -> bool:
        """Check whether the stream has hit the end-of-file"""
        if self._open_mode == OpenMode.Write:
            return False

        # Try to peek one byte
        dummy = self._file.read(1)
        if len(dummy) == 0:
            return True

        # Undo read operation
        self._file.seek(-1, 1)
        return False

    def seek(self, origin: int, offset: int):
        """Seek the stream position"""
        assert origin >= 0 and origin < SeekOrigin.Max

        # No file open
        if self._file == None:
            return

        self._file.seek(offset, origin)

    def open(self, path: str, open_mode: int, data_mode: int):
        """Open stream to file"""
        assert open_mode >= 0 and open_mode < OpenMode.Max
        assert data_mode >= 0 and data_mode < DataMode.Max

        # Close existing file
        if self._file != None:
            self.close()

        self._path = path
        self._open_mode = open_mode
        self._data_mode = data_mode

        self._file = open(self._path, self.__get_py_open_mode())

    def close(self):
        """Close stream"""
        if self._file != None:
            self._file.close()
            self._file = None

    def __get_py_open_mode(self) -> str:
        """Get openmode string based on stream setup"""
        # Read/write mode
        mode = ("r", "w")[self._open_mode]

        # Binary data
        if self._data_mode == DataMode.Binary:
            mode += "b"

        return mode
