from libkiwi_py.binary.types.member import Member
from libkiwi_py.stream.stream_base import StreamBase


class Primitive(Member):
    """Primitive-type member"""

    PRIM_2_WRITE_FUNC = {
        "u8": StreamBase.write_u8,
        "unsigned char": StreamBase.write_u8,
        "bool": StreamBase.write_u8,

        "s8": StreamBase.write_s8,
        "char": StreamBase.write_s8,
        "signed char": StreamBase.write_s8,

        "u16": StreamBase.write_u16,
        "unsigned short": StreamBase.write_u16,
        "wchar_t": StreamBase.write_u16,

        "s16": StreamBase.write_s16,
        "short": StreamBase.write_s16,
        "signed short": StreamBase.write_s16,

        "u32": StreamBase.write_u32,
        "unsigned long": StreamBase.write_u32,

        "s32": StreamBase.write_s32,
        "long": StreamBase.write_s32,
        "signed long": StreamBase.write_s32,
        "int": StreamBase.write_s32,
        "signed int": StreamBase.write_s32,

        "f32": StreamBase.write_f32,
        "float": StreamBase.write_f32,

        "f64": StreamBase.write_f64,
        "double": StreamBase.write_f64,
    }

    PRIM_2_BYTE_SIZE = {
        "u8": 1,
        "unsigned char": 1,
        "bool": 1,

        "s8": 1,
        "char": 1,
        "signed char": 1,

        "u16": 2,
        "unsigned short": 2,
        "wchar_t": 2,

        "s16": 2,
        "short": 2,
        "signed short": 2,

        "u32": 4,
        "unsigned long": 4,

        "s32": 4,
        "long": 4,
        "signed long": 4,
        "int": 4,
        "signed int": 4,

        "f32": 4,
        "float": 4,

        "f64": 8,
        "double": 8,
    }

    def __init__(self, _type: str, name: str, arr: str = None, value=None):
        super().__init__(_type, name, arr, value)

        # Attempt to use default primitive value if nothing was specified
        if self.value == None:
            self.value = self._get_default_value()

    def byte_size(self):
        """Size of primitive member in bytes"""
        assert self.type in self.PRIM_2_BYTE_SIZE, "Not a primitive type!"
        size_one = self.PRIM_2_BYTE_SIZE[self.type]
        return size_one * self.array_size()

    def write(self, strm: StreamBase):
        """Write primitive member to stream"""
        assert self.type in self.PRIM_2_BYTE_SIZE, "Not a primitive type!"
        f = self.PRIM_2_WRITE_FUNC[self.type]

        for elem in self:
            f(strm, elem)

    def _get_default_value(self):
        """Get default value for primitive member"""
        # Variable-length array
        if self.is_vl_array():
            # Cannot safely initialize, user doesn't know length
            return []

        default = None

        if self.type in self.PRIM_2_WRITE_FUNC:
            default = 0
        # Unsupported type, or is a Structure
        else:
            print(
                f"[WARN] Primitive type has no known default value: {self.type}")
            return None

        # Scale for arrays
        if self.is_array():
            return [default] * self.array_size()

        # Single element
        return default
