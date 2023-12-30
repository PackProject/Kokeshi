from libkiwi_py.stream.stream_base import StreamBase
from libkiwi_py.utility.util import Util
from re import search


class Member:
    """Representation of a structure field/member"""

    ARRAY_REGEX = r"\[\s*(?P<Size>[0-9]*)\s*\]"

    def __init__(self, _type: str, name: str, arr: str = None, value=None):
        # Type name
        self.type = _type
        # Member name
        self.name = name

        # Array declaration
        if arr != None:
            match_obj = search(self.ARRAY_REGEX, arr)
            assert match_obj != None and match_obj.group(
                "Size") != None, f"Invalid array declaration: {arr}"

            self.__length = match_obj.group("Size")

            # Variadic-length array
            if self.__length == "":
                self.__length = -1
            # Explicit length
            else:
                # Limit array length to 1
                self.__length = max(1, int(self.__length))

        # Single element
        else:
            self.__length = 1

        # Member value
        self.set_value(value)

    def __repr__(self):
        """Convert object to string (for debugging)"""
        text = f"{self.type} {self.name}"

        # Format array type
        if self.is_array():
            # Variable-length array
            if self.is_vl_array():
                text += "[]"
            # Explicit length
            else:
                text += f"[{self.__length}]"

        return f"{text} = {self.value}"

    def __iter__(self):
        """Iterator support"""
        if self.is_array():
            return self.value.__iter__()
        return [self.value].__iter__()

    def set_value(self, data):
        """Set member value"""
        # Everything is nullable
        if data == None:
            if self.is_vl_array():
                self.value = list()
            else:
                self.value = data
            return

        # Is an array
        if self.is_array():
            assert Util.is_iterable(
                data), "Cannot set non-array value to array member"

            # Explicit array length
            if not self.is_vl_array():
                assert len(
                    data) == self.__length, f"Array length mismatch: got {len(data)}, expected {self.__length}"

            # Convert all iterable types to list
            self.value = list(data)

        # Not an array
        else:
            assert not Util.is_list(
                data) or len(data) == 1, "Cannot set list value to non-array member"
            self.value = data

    def array_size(self):
        """Member array length"""
        if self.is_vl_array():
            # VLA is not yet initialized
            if self.value == None:
                return 1
            # VLA has been initialized
            return len(self.value)
        if self.is_array():
            return self.__length
        return 1

    def byte_size(self):
        """Size of member in bytes"""
        pass

    def write(self, strm: StreamBase):
        """Write member to stream"""
        pass

    def is_array(self):
        """Check whether the member is an array"""
        return self.__length == -1 or self.__length > 1

    def is_vl_array(self):
        """Check whether the member is a variable-length array"""
        return self.__length == -1
