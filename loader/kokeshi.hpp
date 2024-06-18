//
// Kokeshi
//
// Framework for injecting custom code into Pack Project titles
// Available for use under MIT license
//

#ifndef KOKESHI_H
#define KOKESHI_H
#include <types.h>

//! @addtogroup kokeshi
//! @{

#ifdef KOKESHI_PARSED_BY_DOXYGEN
/**
 * @brief Conditional compilation
 *
 * @param sports Code to compile under PACK_SPORTS
 * @param play Code to compile under PACK_PLAY
 * @param resort Code to compile under PACK_RESORT
 */
#define KOKESHI_BY_PACK(sports, play, resort)
#elif PACK_SPORTS
#define KOKESHI_BY_PACK(sports, play, resort) sports
#elif PACK_PLAY
#define KOKESHI_BY_PACK(sports, play, resort) play
#elif PACK_RESORT
#define KOKESHI_BY_PACK(sports, play, resort) resort
#else
#error Game not selected!
#endif

//! Null statement for KOKESHI_BY_PACK
#define KOKESHI_BY_PACK_NOOP ((void)0)
//! Null statement for KOKESHI_BY_PACK
#define KOKESHI_NOTIMPLEMENTED KOKESHI_BY_PACK_NOOP

// clang-format off
//! Name of the module-related files
#define KOKESHI_MODULE_PATH                                                    \
    KOKESHI_BY_PACK("/modules/m_sports", /* Wii Sports */                      \
                    "/modules/m_play",   /* Wii Play */                        \
                    "/modules/m_resort") /* Wii Sports Resort */
// clang-format on

//! @}

namespace kokeshi {
//! @addtogroup kokeshi
//! @{

/**
 * @brief Kamek module information
 */
struct ModuleInfo {
    const void* start; //!< Starting address of the module binary
    u32 size;          //!< Size of the module binary
};

//! Information about the currently loaded Kamek module
ModuleInfo sModuleInfo : 0x80003200;

//! Path to the module's code binary
static const char* scModulePath = KOKESHI_MODULE_PATH ".bin";
//! Path to the module's mapfile
static const char* scMapfilePath = KOKESHI_MODULE_PATH ".map";

/**
 * @brief Allocates memory
 *
 * @param size Block size
 * @param sys Use system (MEM1) heap
 */
void* Alloc(size_t size, bool sys);

/**
 * @brief Frees memory
 *
 * @param pBlock Memory block
 * @param sys Use system (MEM1) heap
 */
void Free(void* pBlock, bool sys);

//! @}
} // namespace kokeshi

#endif
