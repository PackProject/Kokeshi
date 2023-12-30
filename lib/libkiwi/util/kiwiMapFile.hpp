#ifndef LIBKIWI_UTIL_MAP_FILE_H
#define LIBKIWI_UTIL_MAP_FILE_H
#include <libkiwi/core/kiwiLinkList.hpp>
#include <libkiwi/util/kiwiDynamicSingleton.hpp>
#include <types.h>

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
    enum LinkType {
        LinkType_None,       // Map file not loaded
        LinkType_Static,     // Map file for static module
        LinkType_Relocatable // Map file for dynamic/relocatable module
    };

    /**
     * Map file symbol
     */
    struct Symbol {
        // Symbol linkage
        LinkType type;
        // Symbol location
        union {
            void* addr;
            u32 offset;
        };
        // Symbol size
        u32 size;
        // Symbol name
        char* name;
    };

public:
    /**
     * Tests whether a map file has been loaded and unpacked
     */
    bool IsAvailable() const {
        return mpMapBuffer != NULL && mIsUnpacked;
    }

    void Open(String path, LinkType type);
    void Close();

    const Symbol* QueryTextSymbol(const void* addr);

private:
    MapFile();
    ~MapFile();

    void Unpack();

private:
    // Map file type
    LinkType mLinkType;
    // Map file text buffer
    char* mpMapBuffer;
    // Whether the map has been unpacked
    bool mIsUnpacked;
    // Map symbols
    TList<Symbol> mSymbols;
};

} // namespace kiwi

#endif
