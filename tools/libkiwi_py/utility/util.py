class Util:
    """General utility functions"""

    @staticmethod
    def is_iterable(object) -> bool:
        """Determine whether the object is iterable"""
        try:
            iter(object)
            return True
        except TypeError:
            return False

    @staticmethod
    def is_list(object) -> bool:
        """Determine whether the object a list"""
        return isinstance(object, list)

    @staticmethod
    def str_len(string: str, terminator: bool = False):
        """String length"""
        _len = len(string)
        return _len + 1 if terminator else _len

    @staticmethod
    def wstr_len(string: str, terminator: bool = False):
        """Widechar (UTF-16) string length"""
        return Util.str_len(string, terminator) * 2  # wchar_t
