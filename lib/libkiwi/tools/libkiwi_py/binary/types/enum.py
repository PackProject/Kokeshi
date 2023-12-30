from libkiwi_py.binary.types.primitive import Primitive


class Enum(Primitive):
    """Primitive member associated with an enum"""

    def __init__(self, underlying_type: str, name: str, arr: str = None, value=None):
        super().__init__(underlying_type, name, arr, value)
        self.__enum = self.VALUES

    def __getitem__(self, key: str):
        """Access enum values like a dictionary"""
        assert key in self.__enum, f"Invalid enum value: {key} not in {self.__class__.__name__}"
        return self.__enum[key]

    def set_value(self, data):
        """Set member value"""
        if data == None:
            data = self._get_default_value()

        # Enum value
        if type(data) == str:
            assert data in self.__enum, f"Invalid enum value: {data} not in {self.__class__.__name__}"
            Primitive.set_value(self, self.__enum[data])

        # Raw value (sure, I guess..)
        else:
            assert type(
                data) == int, "Specify either an enum value or a raw integer value."
            Primitive.set_value(self, data)
