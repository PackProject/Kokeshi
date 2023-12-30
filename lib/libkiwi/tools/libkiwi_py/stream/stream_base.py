from abc import ABC
from struct import pack, unpack


class SeekOrigin:
    Begin, Current, End, Max = range(4)


class Endian:
    Little, Big, Max = range(3)


class StreamBase(ABC):
    """Base stream class

    Only read/write/eof/seek are required for derived class implementations.
    However, specialization of other methods is possible if necessary.
    """

    def __init__(self, endian: int):
        assert endian >= 0 and endian < Endian.Max
        self._endian = endian

    def read(self, size: int) -> bytes:
        """Read bytes from stream"""
        pass

    def write(self, data: bytes):
        """Write bytes to stream"""
        pass

    def eof(self) -> bool:
        """Check whether the stream has hit the end-of-file"""
        pass

    def seek(self, origin: int, offset: int):
        """Seek the stream position"""
        pass

    def close(self):
        """Close stream"""
        pass

    def read_s8(self) -> int:
        """Read a signed 8-bit integer from the stream."""
        return self.__bytes2int(self.read(1), signed=True)

    def write_s8(self, data: int):
        """Write a signed 8-bit integer to the stream."""
        return self.write(self.__int2bytes(data, size=1, signed=True))

    def read_u8(self) -> int:
        """Read a unsigned 8-bit integer from the stream."""
        return self.__bytes2int(self.read(1), signed=False)

    def write_u8(self, data: int):
        """Write a unsigned 8-bit integer to the stream."""
        return self.write(self.__int2bytes(data, size=1, signed=False))

    def read_s16(self) -> int:
        """Read a signed 16-bit integer from the stream."""
        return self.__bytes2int(self.read(2), signed=True)

    def write_s16(self, data: int):
        """Write a signed 16-bit integer to the stream."""
        return self.write(self.__int2bytes(data, size=2, signed=True))

    def read_u16(self) -> int:
        """Read a unsigned 16-bit integer from the stream."""
        return self.__bytes2int(self.read(2), signed=False)

    def write_u16(self, data: int):
        """Write a unsigned 16-bit integer to the stream."""
        return self.write(self.__int2bytes(data, size=2, signed=False))

    def read_s32(self) -> int:
        """Read a signed 32-bit integer from the stream."""
        return self.__bytes2int(self.read(4), signed=True)

    def write_s32(self, data: int):
        """Write a signed 32-bit integer to the stream."""
        return self.write(self.__int2bytes(data, size=4, signed=True))

    def read_u32(self) -> int:
        """Read a unsigned 32-bit integer from the stream."""
        return self.__bytes2int(self.read(4), signed=False)

    def write_u32(self, data: int):
        """Write a unsigned 32-bit integer to the stream."""
        return self.write(self.__int2bytes(data, size=4, signed=False))

    def read_f32(self) -> int:
        """Read a single-precision floating-point value from the stream."""
        return self.__bytes2dec(self.read(4))

    def write_f32(self, data: int):
        """Write a single-precision floating-point value to the stream."""
        return self.write(self.__dec2bytes(data, size=4))

    def read_f64(self) -> int:
        """Read a double-precision floating-point value from the stream."""
        return self.__bytes2dec(self.read(8))

    def write_f64(self, data: int):
        """Write a double-precision floating-point value to the stream."""
        return self.write(self.__dec2bytes(data, size=8))

    def read_string(self, maxlen: int = -1) -> str:
        """Read a string (ANSI) from the stream."""
        string = ""
        i = 0

        while maxlen < 0 or i < maxlen:
            data = self.read(1)

            # Null terminator
            if data[0] == 0x00:
                break

            # Append new character
            try:
                string += data.decode("utf-8")
            except UnicodeDecodeError:
                break

            i += 1

        return string

    def write_string(self, string: str, maxlen: int = -1, terminate: bool = True):
        """Write a string (ANSI) to the stream."""
        if maxlen >= 0:
            # Reserve last space for null terminator
            term_size = 1 if terminate else 0
            # Truncate if string is too long
            string = string[:maxlen - term_size]

        # Write data
        try:
            self.write(string.encode("utf-8"))
        except UnicodeEncodeError:
            return

        # Null terminator
        if terminate:
            self.write_s8(0x00)

    def read_wstring(self, maxlen: int) -> str:
        """Read a widechar string (UTF-16) from the stream."""
        string = ""
        i = 0

        while maxlen > 0 and i < maxlen:
            data = self.read(2)

            # Null terminator
            if data[0] == 0x00 and data[1] == 0x00:
                break

            # Append new character
            try:
                string += data.decode("utf-16")
            except UnicodeDecodeError:
                break

            i += 1

        return string

    def write_wstring(self, string: str, maxlen: int = -1, terminate: bool = True):
        """Write a widechar string (UTF-16) to the stream."""
        # Truncate if string is too long
        if maxlen >= 0:
            # Reserve last space for null terminator
            term_size = 1 if terminate else 0
            # Truncate if string is too long
            string = string[:maxlen - term_size]

        # Write data
        try:
            self.write(string.encode("utf-16-be"))
        except UnicodeEncodeError:
            return

        # Null terminator
        self.write_u16(0x0000)

    def write_padding(self, size: int):
        """Write padding to the stream"""
        self.write(bytes([0x00] * size))

    def __int2bytes(self, data: int, size: int, signed: bool) -> bytes:
        """Convert integer value into bytes"""
        endian = ("little", "big")[self._endian]
        return int.to_bytes(data, length=size, byteorder=endian, signed=signed)

    def __bytes2int(self, data: bytes, signed: bool) -> int:
        """Convert bytes into integer value"""
        endian = ("little", "big")[self._endian]
        return int.from_bytes(data, byteorder=endian, signed=signed)

    def __dec2bytes(self, data: float, size: int) -> bytes:
        """Convert decimal value into bytes"""
        endian = ("<", ">")[self._endian]
        t = "d" if size == 8 else "f"
        arr = pack(f"{endian}{t}", data)
        return bytes(arr)

    def __bytes2dec(self, data: bytes) -> float:
        """Convert bytes into decimal value"""
        endian = ("<", ">")[self._endian]
        t = "d" if len(data) == 8 else "f"
        arr = unpack(f"{endian}{t}", data)
        return bytes(arr)
