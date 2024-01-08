#include <cstring>
#include <egg/core.h>
#include <libkiwi.h>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(MapFile);

/**
 * Constructor
 */
MapFile::MapFile()
    : mLinkType(LinkType_None), mpMapBuffer(NULL), mIsUnpacked(false) {}

/**
 * Destructor
 */
MapFile::~MapFile() {
    Close();
}

/**
 * Opens a map file from the DVD
 * @param path Map file path
 * @param type Module linkage type
 */
void MapFile::Open(String path, LinkType type) {
    K_ASSERT(type != LinkType_None);

    // Close existing map file
    if (mpMapBuffer != NULL) {
        Close();
    }

    // Load file from disc
    mpMapBuffer = static_cast<char*>(EGG::DvdRipper::loadToMainRAM(
        path, NULL, NULL, EGG::DvdRipper::ALLOC_HEAD, 0, NULL, NULL));

    if (mpMapBuffer == NULL) {
        K_LOG_EX("Map file (%s) could not be opened!", path.CStr());
        return;
    }

    mLinkType = type;
    Unpack();
}

/**
 * Closes map file
 */
void MapFile::Close() {
    TList<Symbol>::Iterator it = mSymbols.Begin();
    for (; it != mSymbols.End(); it++) {
        TList<Symbol>::Iterator next = it++;
        mSymbols.Erase(next);
        delete &*next;
    }

    delete mpMapBuffer;
    mpMapBuffer = NULL;

    mIsUnpacked = false;
}

/**
 * Queries text section symbol
 * @param addr Symbol address
 */
const MapFile::Symbol* MapFile::QueryTextSymbol(const void* addr) {
    if (!IsAvailable()) {
        return NULL;
    }

    TList<Symbol>::Iterator it = mSymbols.Begin();
    for (; it != mSymbols.End(); it++) {
        // Resolve the symbol's address
        const void* symb = it->type == LinkType_Static
                               ? it->addr
                               : AddToPtr(GetTextStart(), it->offset);

        // Determine if the specified address falls within the symbol
        if (PtrDistance(symb, addr) < it->size) {
            return &*it;
        }
    }

    return NULL;
}

/**
 * Unpacks loaded map file
 */
void MapFile::Unpack() {
    // Don't unpack twice
    if (IsAvailable()) {
        return;
    }

    // Nothing to unpack
    if (mpMapBuffer == NULL) {
        return;
    }

    // Skip map file header (2 lines)
    char* map = mpMapBuffer;
    for (int i = 0; i < 2; i++) {
        map = ksl::strchr(map, '\n') + 1;
    }

    // Parse lines
    for (char* next = map; (next = ksl::strchr(map, '\n')); map = next + 1) {
        Symbol* sym = new Symbol();

        // Location
        if (mLinkType == LinkType_Static) {
            sym->addr = reinterpret_cast<void*>(ksl::strtoul(map, &map, 16));
        } else {
            sym->offset = ksl::strtoul(map, &map, 16);
        }

        // Linkage
        sym->type = mLinkType;

        // Size
        sym->size = ksl::strtoul(map, &map, 16);

        // Trim whitespace from name
        while (*map == ' ') {
            map++;
        }
        sym->name = map;

        // Terminate symbol string
        *next = '\0';
        // Remove carriage return
        if (*(next - 1) == '\r') {
            *(next - 1) = '\0';
        }

        mSymbols.PushBack(sym);
    }

    mIsUnpacked = true;
}

} // namespace kiwi
