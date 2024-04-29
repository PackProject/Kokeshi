from libkiwi_py.binary.types.member import Member
from libkiwi_py.stream.stream_base import StreamBase
from copy import deepcopy
from libkiwi_py.utility.util import Util


class Structure(Member):
    """Structure containing members"""

    def __init__(self, name: str, arr: str = None, values=None):
        super().__init__(self.__class__.__name__, name, arr)

        # Convert member list into dictionary
        members = {m.name: m for m in deepcopy(self.MEMBERS)}

        if self.is_array():
            # Cannot initialize variable length array
            if self.is_vl_array():
                self.value = list()
            else:
                # In an array, all indices will be initialized the same way
                self.value = [members] * self.array_size()
        # Single element
        else:
            self.value = members

        # User initialization
        if values != None:
            self.set_value(values)

    def __getitem__(self, key):
        """Access member of structure"""
        return self.value[key]

    def __repr__(self):
        """Convert object to string (for debugging)"""
        return f"{self.type} {self.name} = {{{','.join(self.value)}}}"

    def byte_size(self):
        """Size of structure in bytes"""
        # Size of one instance
        size = sum(m.byte_size() for m in self.MEMBERS)
        # Scale for array
        return size * self.array_size()

    def write(self, strm: StreamBase):
        """Write structure to stream"""
        for inst in self:
            for m in inst.values():
                m.write(strm)

    def append(self, member: Member):
        """Append to structure's array"""
        assert self.is_array(), "Not an array!"
        self.value.append(member.value)

    def offset_of(self, key: str):
        """Offset of member in structure"""
        # First set of members
        member_dict = self.value if not self.is_array(
        ) else self.value[0]

        # Add up offsets until we hit the desired member
        offset = 0
        for item in member_dict.items():
            # Target member found
            if item[0].name == key:
                return offset
            # Keep going
            offset += item[1].byte_size()

        # Member is not in structure
        return -1

    def set_value(self, members):
        """Set structure values (members)"""
        if members == None:
            return

        # Convert to length 1 list
        if not Util.is_list(members):
            members = [members]

        # Structure members as list
        my_members = [self.value] if not self.is_array() else self.value

        # Length should be the same
        assert len(members) == len(
            my_members), f"Invalid array initialization: Expected {len(my_members)} entries, got {len(members)}"

        # User initialization
        for i, m in enumerate(members):
            # Array element (i'th instance of structure)
            inst = my_members[i]

            # Apply values
            for item in m.items():
                k, v = item[0], item[1]
                if k in inst:
                    inst[k].set_value(v)
                else:
                    print(
                        f"[WARN] Cannot access undeclared member: {self.__class__.__name__}::{k}. Value will be ignored")
