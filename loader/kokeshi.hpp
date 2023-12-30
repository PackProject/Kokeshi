/**
 * Kokeshi
 *
 * Framework for injecting custom code into Pack Project titles
 *
 * Available for use under MIT license
 */

#ifndef KOKESHI_H
#define KOKESHI_H
#include <types.h>

/**
 * @brief Conditional compilation, by pack
 */
#ifdef PACK_SPORTS
#define KOKESHI_BY_PACK(sports, play, resort) sports
#elif PACK_PLAY
#define KOKESHI_BY_PACK(sports, play, resort) play
#elif PACK_RESORT
#define KOKESHI_BY_PACK(sports, play, resort) resort
#else
#error Game not selected!
#endif

/**
 * @brief Null statement for KOKESHI_BY_PACK
 */
#define KOKESHI_BY_PACK_NOOP ((void)0)

/**
 * @brief Kamek module name
 */
// clang-format off
#define KOKESHI_MODULE_PATH                                                    \
    KOKESHI_BY_PACK("/modules/m_sports", /* Wii Sports */                      \
                    "/modules/m_party",  /* Wii Play */                        \
                    "/modules/m_resort") /* Wii Sports Resort */
// clang-format on

namespace kokeshi {

/**
 * @brief Path to code module
 */
static const char* scModulePath = KOKESHI_MODULE_PATH ".bin";

/**
 * @brief Path to module mapfile
 */
static const char* scMapfilePath = KOKESHI_MODULE_PATH ".map";

void* Alloc(std::size_t size, bool sys);
void Free(void* block, bool sys);

} // namespace kokeshi

#endif
