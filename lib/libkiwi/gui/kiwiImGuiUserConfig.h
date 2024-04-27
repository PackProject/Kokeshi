#ifndef LIBKIWI_GUI_IMGUI_USER_CONFIG_H
#define LIBKIWI_GUI_IMGUI_USER_CONFIG_H
#include <libkiwi/kernel/kiwiAssert.h>
#include <libkiwi/math/kiwiAlgorithm.h>
#include <libkiwi/prim/kiwiSTL.h>
#include <types.h>

// Disable incompatible functionality
#define IMGUI_DISABLE_WIN32_FUNCTIONS
#define IMGUI_DISABLE_DEFAULT_ALLOCATORS
#define IMGUI_DISABLE_SSE
#define IMGUI_DISABLE_FILE_FUNCTIONS

// Redirect ImGui debug output
#define IM_ASSERT(expr) K_ASSERT(expr)
#define IM_DEBUG_BREAK() IM_ASSERT(0)
#define IMGUI_DEBUG_PRINTF(_FMT, ...) K_LOG_EX(_FMT, __VA_ARGS__)

// STL qsort not linked in our games
#define ImQsort ksl::qsort

#endif
