#include <libkiwi.h>

#include <cstdlib>
#include <cstring>

namespace kiwi {

K_DYNAMIC_SINGLETON_IMPL(MapFile);

/**
 * @brief Constructor
 */
MapFile::MapFile()
    : mLinkType(ELinkType_None), mpMapBuffer(nullptr), mIsUnpacked(false) {}

/**
 * @brief Destructor
 */
MapFile::~MapFile() {
    Close();
}

/**
 * @brief Opens a map file from the DVD
 *
 * @param rPath Map file path
 * @param type Module linkage type
 */
void MapFile::Open(const String& rPath, ELinkType type) {
    K_ASSERT(type != ELinkType_None);

    // Close existing map file
    if (mpMapBuffer != nullptr) {
        Close();
    }

    // Try to open file on the DVD
    mpMapBuffer = static_cast<char*>(FileRipper::Rip(rPath, EStorage_DVD));
    if (mpMapBuffer == nullptr) {
        K_LOG_EX("Map file (%s) could not be opened!\n", rPath.CStr());
        return;
    }

    mLinkType = type;
    Unpack();
}

/**
 * @brief Closes map file
 */
void MapFile::Close() {
    TList<Symbol>::Iterator it = mSymbols.Begin();
    for (; it != mSymbols.End(); it++) {
        TList<Symbol>::Iterator next = it++;
        mSymbols.Erase(next);
        delete &*next;
    }

    delete mpMapBuffer;
    mpMapBuffer = nullptr;

    mIsUnpacked = false;
}

/**
 * @brief Queries text section symbol
 *
 * @param pAddr Symbol address
 */
const MapFile::Symbol* MapFile::QueryTextSymbol(const void* pAddr) const {
    if (!IsAvailable()) {
        return nullptr;
    }

    TList<Symbol>::ConstIterator it = mSymbols.Begin();
    for (; it != mSymbols.End(); it++) {
        // Resolve the symbol's address
        const void* pResolved =
            it->type == ELinkType_Static
                ? it->pAddr
                : AddToPtr(GetModuleTextStart(), it->offset);

        // Determine if the specified address falls within the symbol
        if (PtrDistance(pResolved, pAddr) < it->size) {
            return &*it;
        }
    }

    return nullptr;
}

/**
 * @brief Unpacks loaded map file
 */
void MapFile::Unpack() {
    K_ASSERT(mpMapBuffer != nullptr);

    // Skip map file header (2 lines)
    char* pIt = mpMapBuffer;
    for (int i = 0; i < 2; i++) {
        pIt = std::strchr(pIt, '\n') + 1;
    }

    // Parse lines
    for (char* pEndl = pIt; (pEndl = std::strchr(pIt, '\n')); pIt = pEndl + 1) {
        Symbol* sym = new Symbol();
        K_ASSERT(sym != nullptr);

        // Location
        if (mLinkType == ELinkType_Static) {
            sym->pAddr = reinterpret_cast<void*>(std::strtoul(pIt, &pIt, 16));
        } else {
            sym->offset = std::strtoul(pIt, &pIt, 16);
        }

        // Linkage
        sym->type = mLinkType;

        // Size
        sym->size = std::strtoul(pIt, &pIt, 16);

        // Trim whitespace from name
        while (*pIt == ' ') {
            pIt++;
        }
        sym->pName = pIt;

        // Terminate symbol string
        *pEndl = '\0';
        // Remove carriage return
        if (*(pEndl - 1) == '\r') {
            *(pEndl - 1) = '\0';
        }

        mSymbols.PushBack(sym);
    }

    mIsUnpacked = true;
}

} // namespace kiwi
