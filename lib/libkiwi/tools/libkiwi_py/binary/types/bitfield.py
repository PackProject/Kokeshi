from libkiwi_py.binary.types.member import Member
from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.stream.stream_base import StreamBase
from copy import deepcopy


class BitMember:
    """Member of bitfield"""

    def __init__(self, name: str, size: int, value=0, enum_cls=None):
        self.name = name
        self.size = size
        self.value = value

        # Hacky, but we sometimes the bitfield member should take on an enum value.
        # We don't want inheritance, because these enums should already exist.
        self.__enum = enum_cls.VALUES if enum_cls != None else dict()

        self.set_value(value)

    def set_value(self, value):
        """Set member value, potentially using an enum value"""
        # Enum value
        if type(value) == str:
            assert value in self.__enum, f"Invalid enum value for {self.name}: {value}"
            self.value = self.__enum[value]

        # Raw value
        else:
            assert type(
                value) == int, "Specify either an enum value or a raw integer value."
            self.value = value


class BitField(Member):
    """Bit field member"""

    def __init__(self, underlying_type: str, name: str, values: dict[str, int] = {}):
        super().__init__(self.__class__.__name__, name)
        self.__type = underlying_type

        # Convert member list into dictionary
        self.value = {m.name: m for m in deepcopy(self.MEMBERS)}

        # User initialization
        for item in values.items():
            k, v = item[0], item[1]
            if k in self.members:
                self.members[k].set_value(v)

        assert self.__type in Primitive.PRIM_2_WRITE_FUNC, f"Invalid bitfield type: {self.__type}"

        self.__validate()

    def __getitem__(self, key: str):
        """Access member of bitfield by name"""
        assert key in self.__members, f"Member {key} not in bitfield {self.__class__.__name__}"
        return self.__members[key]

    def __iter__(self):
        """Iterator support"""
        return self.value.values().__iter__()

    def byte_size(self):
        """Size of bitfield in bytes"""
        assert self.__type in Primitive.PRIM_2_BYTE_SIZE, f"Invalid bitfield type: {self.__type}"
        return Primitive.PRIM_2_BYTE_SIZE[self.__type]

    def write(self, strm: StreamBase):
        """Write bitfield to file"""
        assert self.__type in Primitive.PRIM_2_WRITE_FUNC, f"Invalid bitfield type: {self.__type}"
        f = Primitive.PRIM_2_WRITE_FUNC[self.__type]

        f(strm, self.__raw_value())

    def set_value(self, data):
        """Set bitfield values"""
        # Everything is nullable
        if data == None:
            return

        # User initialization
        for item in data.items():
            k, v = item[0], item[1]

            if k in self.value:
                self.value[k].set_value(v)
            else:
                print(
                    f"[WARN] Cannot access undeclared member: {self.__class__.__name__}::{k}. Value will be ignored")

    def __validate(self):
        """Validate members of bitfield"""
        max_bytes = self.byte_size()
        my_bits = sum(member.size for member in self)

        # Get byte count
        my_bytes = my_bits // 8

        # Roll over into next byte
        if my_bits % 8 != 0:
            my_bytes += 1

        assert my_bytes <= max_bytes

    def __raw_value(self):
        """Get raw (integer) value of bitfield"""
        raw = 0
        pos = 0

        # Compose value
        for member in self:
            assert pos < self.byte_size() * 8, "Bitfield overflow!!"

            mask = (~(1 << member.size))
            value = member.value & mask
            raw |= (value << pos)

            pos += member.size

        return raw
