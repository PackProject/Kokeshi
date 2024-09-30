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
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80183f18, Report),  // Wii Sports
                KM_BRANCH(0x80183b50, Report),  // Wii Play
                KM_BRANCH(0x80235cc8, Report)); // Wii Sports Resort
// clang-format on

/**
 * @brief Loads Kamek module and applies patches
 */
void Load() {
    kamek::loadKamekBinaryFromDisc(&cLoaderFunctions, BINARY_PATH);
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x80183098, Load),  // Wii Sports
                KM_BRANCH(0x80182d6c, Load),  // Wii Play
                KM_BRANCH(0x8022d980, Load)); // Wii Sports Resort
// clang-format on

} // namespace

/**
 * @brief Allocates memory
 *
 * @param size Block size
 * @param sys Use system (MEM1) heap
 */
void* Alloc(size_t size, bool sys) {
    EGG::Heap* pMem1Heap = RPSysSystem::getRootHeapMem1();
    EGG::Heap* pMem2Heap = RPSysSystem::getRootHeapMem2();

    OS_ASSERT(pMem1Heap != nullptr, "MEM1 heap is NULL");
    OS_ASSERT(pMem2Heap != nullptr, "MEM2 heap is NULL");

    return RP_GET_INSTANCE(RPSysSystem)
        ->alloc(sys ? pMem1Heap : pMem2Heap, size, 32);
}

/**
 * @brief Frees memory
 *
 * @param pBlock Memory block
 * @param sys Use system (MEM1) heap
 */
void Free(void* pBlock, bool sys) {
    EGG::Heap* pMem1Heap = RPSysSystem::getRootHeapMem1();
    EGG::Heap* pMem2Heap = RPSysSystem::getRootHeapMem2();

    OS_ASSERT(pMem1Heap != nullptr, "MEM1 heap is NULL");
    OS_ASSERT(pMem2Heap != nullptr, "MEM2 heap is NULL");

    return RP_GET_INSTANCE(RPSysSystem)
        ->free(sys ? pMem1Heap : pMem2Heap, pBlock);
}

} // namespace kokeshi
