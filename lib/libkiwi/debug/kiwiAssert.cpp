#include <cstdio>
#include <cstring>
#include <libkiwi.h>
#include <revolution/OS.h>

/**
 * @brief Logs a message to the console
 *
 * @param pMsg Message
 * @param ... Format string arguments
 */
void kiwi_log(const char* pMsg, ...) {
    char msgbuf[1024];
    std::va_list list;

    va_start(list, pMsg);
    std::vsnprintf(msgbuf, sizeof(msgbuf), pMsg, list);
    va_end(list);

    OSReport(msgbuf);
}

/**
 * @brief Halts the program and displays an error message to the screen/console
 *
 * @param pFile Source file name where assertion failed
 * @param line Source file line where assertion failed
 * @param pMsg Assertion message
 * @param ... Format string arguments
 */
void kiwi_fail_assert(const char* pFile, int line, const char* pMsg, ...) {
    char msgbuf[1024];
    std::va_list list;

    va_start(list, pMsg);
    std::vsnprintf(msgbuf, sizeof(msgbuf), pMsg, list);
    va_end(list);

    kiwi::Nw4rException::GetInstance().FailAssert(pFile, line, msgbuf);
}
// clang-format off
// Catch EGG_ASSERT
KOKESHI_BY_PACK(KM_BRANCH(0x800a1f08, kiwi_fail_assert), // Wii Sports
                KM_BRANCH(0x800a17d8, kiwi_fail_assert), // Wii Play
                );                                      // Wii Sports Resort
                
// Catch OSPanic
KOKESHI_BY_PACK(KM_BRANCH(0x800eefa8, kiwi_fail_assert),  // Wii Sports
                KM_BRANCH(0x800eec30, kiwi_fail_assert),  // Wii Play
                KM_BRANCH(0x80047150, kiwi_fail_assert)); // Wii Sports Resort
// clang-format on
