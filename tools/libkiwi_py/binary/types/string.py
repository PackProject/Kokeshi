from libkiwi_py.binary.types.member import Member
from libkiwi_py.stream.stream_base import StreamBase
from libkiwi_py.utility.util import Util


class String(Member):
    """ANSI string member"""

    STRING_TYPES = ("cstr", "str")

    def __init__(self, name: str, arr: str = None, value=None, c_style: bool = True, maxlen: int = -1, pad: bool = False):
        super().__init__(self.__class__.__name__, name, arr, value)
        self.c_style = c_style
        self.maxlen = maxlen
        self.pad = pad

    def byte_size(self):
        """Size of string member in bytes"""
        # Recurse if array
        if self.is_array():
            return sum(s.byte_size() for s in self)

        # Single string (PADDED)
        if self.pad:
            return self.maxlen

        # Single string (UNPADDED)
        return Util.str_len(self.value, self.c_style)

    def write(self, strm: StreamBase):
        """Write string member to stream"""
        for string in self:
            # String length
            length = Util.str_len(string, terminator=self.c_style)

            # Truncate to specified max length
            if self.maxlen > 0:
                if length > self.maxlen:
                    print(
                        f"[WARN] String {string} will be truncated to {self.maxlen} characters.")

            # Write data
            strm.write_string(string, self.maxlen, self.c_style)

            # Optional padding
            if self.pad and length < self.maxlen:
                strm.write_padding(self.maxlen - length)


class WString(String):
    """Widechar (UTF-16) string member"""

    def __init__(self, name: str, arr: str = None, value=None, c_style: bool = True, maxlen: int = -1, pad: bool = True):
        super().__init__(name, arr, value, c_style, maxlen, pad)

    def byte_size(self):
        """Size of string member in bytes"""
        return String.byte_size() * 2  # wchar_t

    def write(self, strm: StreamBase):
        """Write string member to stream"""
        for string in self:
            # String length
            length = Util.str_len(string, terminator=self.c_style)

            # Truncate to specified max length
            if self.maxlen > 0:
                # NOT A BUG! Max length counts characters, not bytes
                if length > self.maxlen:
                    print(
                        f"[WARN] String {string} will be truncated to {self.maxlen} characters.")

            # Write data
            strm.write_wstring(string, self.maxlen, self.c_style)

            # Optional padding
            if self.pad and length < self.maxlen:
                strm.write_padding(self.maxlen - length)
