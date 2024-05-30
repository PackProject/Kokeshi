#ifndef LIBKIWI_DEBUG_MAP_FILE_H
#define LIBKIWI_DEBUG_MAP_FILE_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>

namespace kiwi {

/**
 * Kamek symbol map utility
 */
class MapFile : public DynamicSingleton<MapFile> {
    friend class DynamicSingleton<MapFile>;

public:
    /**
     * Module link type
     */
    enum ELinkType {
        ELinkType_None,       // Map file not loaded
        ELinkType_Static,     // Map file for static module
        ELinkType_Relocatable // Map file for dynamic/relocatable module
    };

    /**
     * Map file symbol
     */
    struct Symbol {
        ELinkType type; // Linkage
        union {
            void* addr; // Address (unpacked)
            u32 offset; // Offset (packed)
        };
        u32 size;   // Byte size
        char* name; // Mangled name
    };

public:
    /**
     * Tests whether a map file has been loaded and unpacked
     */
    bool IsAvailable() const {
        return mpMapBuffer != NULL && mIsUnpacked;
    }

    void Open(const String& path, ELinkType type);
    void Close();

    const Symbol* QueryTextSymbol(const void* addr);

private:
    MapFile();
    ~MapFile();

    void Unpack();

private:
    ELinkType mLinkType;    // Linkage
    char* mpMapBuffer;      // Text buffer
    bool mIsUnpacked;       // Whether the map has been unpacked
    TList<Symbol> mSymbols; // Map symbols
};

} // namespace kiwi

#endif
