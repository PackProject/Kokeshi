from libkiwi_py.binary.types.structure import Structure
from libkiwi_py.binary.types.primitive import Primitive


class Size(Structure):
    """2D vector representing size"""

    MEMBERS = [
        Primitive("f32", "width"),
        Primitive("f32", "height")
    ]


class VEC2(Structure):
    """2D vector"""

    MEMBERS = [
        Primitive("f32", "x"),
        Primitive("f32", "y")
    ]


class VEC3(Structure):
    """3D vector"""

    MEMBERS = [
        Primitive("f32", "x"),
        Primitive("f32", "y"),
        Primitive("f32", "z")
    ]
