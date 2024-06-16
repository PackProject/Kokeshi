/*
 * Kamek
 * Wii game patching engine
 * (c) Treeki 2010-2018
 */

#include <cstdio>
#include <kamekLoader.hpp>
#include <kokeshi.hpp>
#include <revolution/DVD.h>
#include <revolution/OS.h>

namespace kamek {

struct KBHeader {
    u32 magic1;
    u16 magic2;
    u16 version;
    u32 bssSize;
    u32 codeSize;
    u32 ctorStart;
    u32 ctorEnd;
    u32 _pad[2];
};

#define kAddr32 1
#define kAddr16Lo 4
#define kAddr16Hi 5
#define kAddr16Ha 6
#define kRel24 10
#define kWrite32 32
#define kWrite16 33
#define kWrite8 34
#define kCondWritePointer 35
#define kCondWrite32 36
#define kCondWrite16 37
#define kCondWrite8 38
#define kBranch 64
#define kBranchLink 65

static void kamekError(const char* msg, ...) {
    static const GXColor fg = {255, 255, 255, 255};
    static const GXColor bg = {0, 0, 0, 255};

    char buffer[512];
    std::va_list list;

    va_start(list, msg);
    std::vsnprintf(buffer, sizeof(buffer), msg, list);
    va_end(list);

    OSFatal(fg, bg, buffer);
}

static inline u32 resolveAddress(u32 text, u32 address) {
    if (address & 0x80000000)
        return address;
    else
        return text + address;
}

#define kCommandHandler(name)                                                  \
    static inline const u8* kHandle##name(const u8* input, u32 text,           \
                                          u32 address)
#define kDispatchCommand(name)                                                 \
    case k##name: input = kHandle##name(input, text, address); break

kCommandHandler(Addr32) {
    u32 target = resolveAddress(text, *(const u32*)input);
    *(u32*)address = target;
    return input + 4;
}
kCommandHandler(Addr16Lo) {
    u32 target = resolveAddress(text, *(const u32*)input);
    *(u16*)address = target & 0xFFFF;
    return input + 4;
}
kCommandHandler(Addr16Hi) {
    u32 target = resolveAddress(text, *(const u32*)input);
    *(u16*)address = target >> 16;
    return input + 4;
}
kCommandHandler(Addr16Ha) {
    u32 target = resolveAddress(text, *(const u32*)input);
    *(u16*)address = target >> 16;
    if (target & 0x8000)
        *(u16*)address += 1;
    return input + 4;
}
kCommandHandler(Rel24) {
    u32 target = resolveAddress(text, *(const u32*)input);
    u32 delta = target - address;
    *(u32*)address &= 0xFC000003;
    *(u32*)address |= (delta & 0x3FFFFFC);
    return input + 4;
}
kCommandHandler(Write32) {
    u32 value = *(const u32*)input;
    *(u32*)address = value;
    return input + 4;
}
kCommandHandler(Write16) {
    u32 value = *(const u32*)input;
    *(u16*)address = value & 0xFFFF;
    return input + 4;
}
kCommandHandler(Write8) {
    u32 value = *(const u32*)input;
    *(u8*)address = value & 0xFF;
    return input + 4;
}
kCommandHandler(CondWritePointer) {
    u32 target = resolveAddress(text, *(const u32*)input);
    u32 original = ((const u32*)input)[1];
    if (*(u32*)address == original)
        *(u32*)address = target;
    return input + 8;
}
kCommandHandler(CondWrite32) {
    u32 value = *(const u32*)input;
    u32 original = ((const u32*)input)[1];
    if (*(u32*)address == original)
        *(u32*)address = value;
    return input + 8;
}
kCommandHandler(CondWrite16) {
    u32 value = *(const u32*)input;
    u32 original = ((const u32*)input)[1];
    if (*(u16*)address == (original & 0xFFFF))
        *(u16*)address = value & 0xFFFF;
    return input + 8;
}
kCommandHandler(CondWrite8) {
    u32 value = *(const u32*)input;
    u32 original = ((const u32*)input)[1];
    if (*(u8*)address == (original & 0xFF))
        *(u8*)address = value & 0xFF;
    return input + 8;
}
kCommandHandler(Branch) {
    *(u32*)address = 0x48000000;
    return kHandleRel24(input, text, address);
}
kCommandHandler(BranchLink) {
    *(u32*)address = 0x48000001;
    return kHandleRel24(input, text, address);
}

inline void cacheInvalidateAddress(u32 address) {
    register u32 addressRegister = address;
    asm {
		dcbst 0, addressRegister
		sync
		icbi 0, addressRegister
    }
}

void loadKamekBinary(const loaderFunctions* funcs, const void* binary,
                     u32 binaryLength) {
    kokeshi::sModuleInfo.start = const_cast<void*>(binary);
    kokeshi::sModuleInfo.size = binaryLength;

    const KBHeader* header = (const KBHeader*)binary;
    if (header->magic1 != 'Kame' || header->magic2 != 'k\0')
        kamekError("FATAL ERROR: Corrupted file, please check your game's "
                   "Kamek files");
    if (header->version != 2) {
        kamekError("FATAL ERROR: Incompatible file (version %d), please "
                   "upgrade your Kamek Loader",
                   header->version);
    }

    OSReport("header: bssSize=%u, codeSize=%u, ctors=%u-%u\n", header->bssSize,
             header->codeSize, header->ctorStart, header->ctorEnd);

    u32 textSize = header->codeSize + header->bssSize;
    u32 text = (u32)funcs->kamekAlloc(textSize, true);

    OSReport("\n\n");
    OSReport("=================================\n");
    OSReport("\n");
    OSReport("[ Kamek Module Info ]\n");
    OSReport("\n");
    OSReport("    textStart:  %08X\n", text);
    OSReport("    textSize:   %08X\n", header->codeSize);
    OSReport("    bssStart:   %08X\n", text + header->codeSize);
    OSReport("    bssSize:    %08X\n", header->bssSize);
    OSReport("    ctorsStart: %08X\n", header->ctorStart);
    OSReport("    ctorsSize:  %08X\n", header->ctorEnd - header->ctorStart);
    OSReport("\n");
    OSReport("MODULE BASE ADDRESS: %p\n", text);
    OSReport("For tools/make_dolphin_map.py\n");
    OSReport("\n");
    OSReport("=================================\n");
    OSReport("\n\n");

    if (!text)
        kamekError("FATAL ERROR: Out of code memory");

    const u8* input = ((const u8*)binary) + sizeof(KBHeader);
    const u8* inputEnd = ((const u8*)binary) + binaryLength;
    u8* output = (u8*)text;

    // Create text + bss sections
    for (u32 i = 0; i < header->codeSize; i++) {
        *output = *(input++);
        cacheInvalidateAddress((u32)(output++));
    }
    for (u32 i = 0; i < header->bssSize; i++) {
        *output = 0;
        cacheInvalidateAddress((u32)(output++));
    }

    while (input < inputEnd) {
        u32 cmdHeader = *((u32*)input);
        input += 4;

        u8 cmd = cmdHeader >> 24;
        u32 address = cmdHeader & 0xFFFFFF;
        if (address == 0xFFFFFE) {
            // Absolute address
            address = *((u32*)input);
            input += 4;
        } else {
            // Relative address
            address += text;
        }

        switch (cmd) {
            kDispatchCommand(Addr32);
            kDispatchCommand(Addr16Lo);
            kDispatchCommand(Addr16Hi);
            kDispatchCommand(Addr16Ha);
            kDispatchCommand(Rel24);
            kDispatchCommand(Write32);
            kDispatchCommand(Write16);
            kDispatchCommand(Write8);
            kDispatchCommand(CondWritePointer);
            kDispatchCommand(CondWrite32);
            kDispatchCommand(CondWrite16);
            kDispatchCommand(CondWrite8);
            kDispatchCommand(Branch);
            kDispatchCommand(BranchLink);
        default: OSReport("Unknown command: %d\n", cmd);
        }

        cacheInvalidateAddress(address);
    }

    __sync();
    __isync();

    typedef void (*Func)(void);
    for (Func* f = (Func*)(text + header->ctorStart);
         f < (Func*)(text + header->ctorEnd); f++) {
        (*f)();
    }
}

void loadKamekBinaryFromDisc(const loaderFunctions* funcs, const char* path) {
    OSReport("{Kamek by Treeki}\nLoading Kamek binary '%s'...\n", path);

    s32 entrynum = DVDConvertPathToEntrynum(path);
    if (entrynum < 0) {
        kamekError("FATAL ERROR: Failed to locate file on the disc: %s", path);
    }

    DVDFileInfo handle;
    if (!DVDFastOpen(entrynum, &handle))
        kamekError("FATAL ERROR: Failed to open file!");

    OSReport("DVD file located: addr=%p, size=%d\n", handle.block.addr,
             handle.size);

    u32 length = handle.size, roundedLength = (handle.size + 0x1F) & ~0x1F;
    void* buffer = funcs->kamekAlloc(roundedLength, false);
    if (!buffer)
        kamekError("FATAL ERROR: Out of file memory");

    DVDReadPrio(&handle, buffer, roundedLength, 0, 2);
    DVDClose(&handle);

    loadKamekBinary(funcs, buffer, handle.size);

    funcs->kamekFree(buffer, false);
    OSReport("All done!\n");
}

} // namespace kamek
