//
// Kokeshi
//
// Framework for injecting custom code into Pack Project titles
// Available for use under MIT license
//

#include <Pack/RPSystem.h>
#include <cstdio>
#include <egg/core.h>
#include <kokeshi.hpp>

namespace kokeshi {
namespace {

//! Memory interface for the Kamek loader
const kamek::loaderFunctions cLoaderFunctions = {Alloc, Free};

/**
 * @brief Logs message to the console
 * @note Re-implements OSReport, which is stubbed out in retail builds
 *
 * @param pMsg Message/format string
 * @param ... Format parameters
 */
void Report(const char* pMsg, ...) {
    std::va_list list;
    va_start(list, pMsg);
    std::vprintf(pMsg, list);
    va_end(list);
}
KOKESHI_BY_PACK(KM_BRANCH(0x80183f18, Report), // Wii Sports
                KM_BRANCH(0x80183b50, Report), // Wii Play
                KOKESHI_NOTIMPLEMENTED);       // Wii Sports Resort

/**
 * @brief Loads Kamek module and applies patches
 */
void Load() {
    kamek::loadKamekBinaryFromDisc(&cLoaderFunctions, scModulePath);
}
KOKESHI_BY_PACK(KM_BRANCH(0x80183098, Load), // Wii Sports
                KM_BRANCH(0x80182d6c, Load), // Wii Play
                KOKESHI_NOTIMPLEMENTED);     // Wii Sports Resort

} // namespace

/**
 * @brief Allocates memory
 *
 * @param size Block size
 * @param sys Use system (MEM1) heap
 */
void* Alloc(size_t size, bool sys) {
    return RP_GET_INSTANCE(RPSysSystem)
        ->alloc(sys ? RPSysSystem::getRootHeapMem1()
                    : RPSysSystem::getRootHeapMem2(),
                size, 32);
}

/**
 * @brief Frees memory
 *
 * @param pBlock Memory block
 * @param sys Use system (MEM1) heap
 */
void Free(void* pBlock, bool sys) {
    return RP_GET_INSTANCE(RPSysSystem)
        ->free(sys ? RPSysSystem::getRootHeapMem1()
                   : RPSysSystem::getRootHeapMem2(),
               pBlock);
}

} // namespace kokeshi
