#include <cstdio>
#include <cstring>
#include <kokeshi.hpp>
#include <libkiwi.h>
#include <revolution/OS.h>

/**
 * Logs message to the console
 * @note Newline is automatically appended
 */
void kiwi_log(const char* msg, ...) {
    char msgbuf[1024];
    std::va_list list;

    va_start(list, msg);
    std::vsnprintf(msgbuf, sizeof(msgbuf), msg, list);
    va_end(list);

    OSReport("%s\n", msgbuf);
}

/**
 * Halts execution and displays assertion error to the screen
 * @note Newline is automatically appended
 */
void kiwi_fail_assert(const char* file, int line, const char* msg, ...) {
    char msgbuf[1024];
    std::va_list list;

    va_start(list, msg);
    std::vsnprintf(msgbuf, sizeof(msgbuf), msg, list);
    va_end(list);

    kiwi::Nw4rException::GetInstance().FailAssert(file, line, msgbuf);
}
// clang-format off
KOKESHI_BY_PACK(KM_BRANCH(0x800a1f08, kiwi_fail_assert), // Wii Sports
                KM_BRANCH(0x800a17d8, kiwi_fail_assert), // Wii Play
                KOKESHI_NOTIMPLEMENTED);                 // Wii Sports Resort
// clang-format on
