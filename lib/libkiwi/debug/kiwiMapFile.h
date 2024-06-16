#ifndef LIBKIWI_DEBUG_MAP_FILE_H
#define LIBKIWI_DEBUG_MAP_FILE_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/util/kiwiDynamicSingleton.h>

namespace kiwi {
//! @addtogroup libkiwi_debug
//! @{

/**
 * @brief Kamek symbol map utility
 */
class MapFile : public DynamicSingleton<MapFile> {
    friend class DynamicSingleton<MapFile>;

public:
    /**
     * @brief Module link type
     */
    enum ELinkType {
        ELinkType_None,       // Map file not loaded
        ELinkType_Static,     // Map file for static module
        ELinkType_Relocatable // Map file for dynamic/relocatable module
    };

    /**
     * @brief Map file symbol
     */
    struct Symbol {
        ELinkType type; // Linkage
        union {
            void* pAddr; // Address (unpacked)
            u32 offset;  // Offset (packed)
        };
        u32 size;    // Byte size
        char* pName; // Mangled name
    };

public:
    /**
     * @brief Tests whether a map file has been loaded and unpacked
     */
    bool IsAvailable() const {
        return mpMapBuffer != nullptr && mIsUnpacked;
    }

    /**
     * @brief Opens a map file from the DVD
     *
     * @param rPath Map file path
     * @param type Module linkage type
     */
    void Open(const String& rPath, ELinkType type);
    /**
     * @brief Closes map file
     */
    void Close();

    /**
     * @brief Queries text section symbol
     *
     * @param pAddr Symbol address
     */
    const Symbol* QueryTextSymbol(const void* pAddr) const;

private:
    /**
     * @brief Constructor
     */
    MapFile();
    /**
     * @brief Destructor
     */
    virtual ~MapFile();

    /**
     * @brief Unpacks loaded map file
     */
    void Unpack();

private:
    ELinkType mLinkType;    // Linkage
    char* mpMapBuffer;      // Text buffer
    bool mIsUnpacked;       // Whether the map has been unpacked
    TList<Symbol> mSymbols; // Map symbols
};

//! @}
} // namespace kiwi

#endif
