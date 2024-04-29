from libkiwi_py.binary.types.enum import Enum
from libkiwi_py.binary.types.primitive import Primitive
from libkiwi_py.binary.types.structure import Structure


class GXAlphaOp(Enum):
    """GX alpha operation"""

    VALUES = {
        "GX_AOP_AND": 0,
        "GX_AOP_OR": 1,
        "GX_AOP_XOR": 2,
        "GX_AOP_XNOR": 3,
    }


class GXCompare(Enum):
    """GX comparison"""

    VALUES = {
        "GX_NEVER": 0,
        "GX_LESS": 1,
        "GX_EQUAL": 2,
        "GX_LEQUAL": 3,
        "GX_GREATER": 4,
        "GX_NEQUAL": 5,
        "GX_GEQUAL": 6,
        "GX_ALWAYS": 7
    }


class GXColorSrc(Enum):
    """GX color source"""

    VALUES = {
        "GX_SRC_REG": 0,
        "GX_SRC_VTX": 1
    }


class GXTexCoordID(Enum):
    """GX texture coordinate ID"""

    VALUES = {
        "GX_TEXCOORD0": 0,
        "GX_TEXCOORD1": 1,
        "GX_TEXCOORD2": 2,
        "GX_TEXCOORD3": 3,
        "GX_TEXCOORD4": 4,
        "GX_TEXCOORD5": 5,
        "GX_TEXCOORD6": 6,
        "GX_TEXCOORD7": 7,
        "GX_TEXCOORD_NULL": 255
    }


class GXTexMapID(Enum):
    """GX texture map ID"""

    VALUES = {
        "GX_TEXMAP0": 0,
        "GX_TEXMAP1": 1,
        "GX_TEXMAP2": 2,
        "GX_TEXMAP3": 3,
        "GX_TEXMAP4": 4,
        "GX_TEXMAP5": 5,
        "GX_TEXMAP6": 6,
        "GX_TEXMAP7": 7,
        "GX_TEXMAP_NULL": 255,
        "GX_TEX_DISABLE": 256
    }


class GXIndTexScale(Enum):
    """GX indirect texture scale"""

    VALUES = {
        "GX_ITS_1": 0,
        "GX_ITS_2": 1,
        "GX_ITS_4": 2,
        "GX_ITS_8": 3,
        "GX_ITS_16": 4,
        "GX_ITS_32": 5,
        "GX_ITS_64": 6,
        "GX_ITS_128": 7,
        "GX_ITS_256": 8
    }


class GXColor(Structure):
    """GX 32-bit color"""

    MEMBERS = [
        Primitive("u8", "r"),
        Primitive("u8", "g"),
        Primitive("u8", "b"),
        Primitive("u8", "a")
    ]


class GXColorS10(Structure):
    """GX 64-bit color"""

    MEMBERS = [
        Primitive("s16", "r"),
        Primitive("s16", "g"),
        Primitive("s16", "b"),
        Primitive("s16", "a")
    ]
