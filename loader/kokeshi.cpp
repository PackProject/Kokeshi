/**
 * Kokeshi
 *
 * Framework for injecting custom code into Pack Project titles
 *
 * Available for use under MIT license
 */

#include <Pack/RPKernel.h>
#include <cstdio>
#include <egg/core.h>
#include <kokeshi.hpp>

namespace kokeshi {
namespace {

/**
 * @brief Memory interface for the Kamek loader
 */
const kamek::loaderFunctions cLoaderFunctions = {Alloc, Free};

/**
 * @brief Get heap from RP engine.
 *
 * @details Memory for the binary sections is allocated from MEM1 (system heap).
 * Temp memory for the Kamek binary is allocated from MEM2 (resource heap).
 *
 * @param mem1 Use MEM1 (MEM2 otherwise)
 */
EGG::Heap* GetHeap(bool mem1) {
    return mem1 ? RPSysSystem::getInstance()->getSystemHeap()
                : RPSysSystem::getInstance()->getResourceHeap();
}

/**
 * @brief Log message
 *
 * @details Re-implements OSReport (stubbed out in retail games)
 *
 * @param msg Message/format string
 * @param ... Format parameters
 */
void Report(const char* msg, ...) {
    std::va_list list;
    va_start(list, msg);
    std::vprintf(msg, list);
    va_end(list);
}
KOKESHI_BY_PACK(KM_BRANCH(0x80183f18, Report), // Wii Sports
                KM_BRANCH(0x80183b50, Report), // Wii Play
                KOKESHI_NOTIMPLEMENTED);       // Wii Sports Resort

/**
 * @brief Load module and apply patches
 */
void Load() {
    kamek::loadKamekBinaryFromDisc(&cLoaderFunctions, scModulePath);
}
KOKESHI_BY_PACK(KM_BRANCH(0x80183098, Load), // Wii Sports
                KM_BRANCH(0x80182d6c, Load), // Wii Play
                KOKESHI_NOTIMPLEMENTED);     // Wii Sports Resort

} // namespace

/**
 * @brief Allocate memory
 *
 * @param size Block size
 * @param sys Use system (MEM1) heap
 */
void* Alloc(std::size_t size, bool sys) {
    return RPSysSystem::getInstance()->alloc(GetHeap(sys), size, 32);
}

/**
 * @brief Free memory
 *
 * @param block Memory block
 * @param sys Use system heap
 */
void Free(void* block, bool sys) {
    return RPSysSystem::getInstance()->free(GetHeap(sys), block);
}

} // namespace kokeshi
