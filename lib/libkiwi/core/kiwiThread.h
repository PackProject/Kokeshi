#ifndef LIBKIWI_CORE_THREAD_H
#define LIBKIWI_CORE_THREAD_H
#include <libkiwi/kernel/kiwiAssert.h>
#include <libkiwi/prim/kiwiBitCast.h>
#include <revolution/OS.h>
#include <types.h>

namespace kiwi {
namespace detail {

/**
 * @brief Common thread implementation
 */
class ThreadImpl {
protected:
    ThreadImpl();
    ~ThreadImpl();

    void Start();
    void Join();

    void SetFunction(const void* addr);
    void SetGPR(u32 i, u32 value);

    template <typename TFunc, typename TClass>
    void SetMemberFunction(TFunc fn, const TClass& obj);

private:
    OSThread* mpOSThread; // RVL thread
    u8* mpThreadStack;    // RVL thread stack

    // OS thread parameters
    static const u32 scStackSize = 0x4000;
    static const s32 scPriority = OS_PRIORITY_MAX / 2;
};

} // namespace detail

/**
 * @brief Similar to std::thread
 */
class Thread : public detail::ThreadImpl {
public:
    // Thread function parameter
    typedef void* Param;

public:
    /**
     * Non-member function
     */

    /**
     * @brief Constructor
     *
     * @param fn Static, no-parameter function
     */
    template <typename TRet> Thread(TRet (*fn)()) {
        K_ASSERT(fn != NULL);

        SetFunction(fn);
        Start();
    }

    /**
     * @brief Constructor
     *
     * @param fn Static, single-parameter function
     * @param arg Function argument
     */
    template <typename TRet> Thread(TRet (*fn)(Param), Param arg) {
        K_ASSERT(fn != NULL);

        SetFunction(fn);
        SetGPR(3, BitCast<u32>(arg));
        Start();
    }

    /**
     * Member function (non-const)
     */

    /**
     * @brief Constructor
     *
     * @param fn No-parameter member function
     * @param obj Class instance
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*fn)(), TClass& obj) {
        K_ASSERT(fn != NULL);

        SetMemberFunction(fn, obj);
        Start();
    }

    /**
     * @brief Constructor
     *
     * @param fn Single-parameter member function
     * @param obj Class instance
     * @param arg Function argument
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*fn)(Param), TClass& obj, Param arg) {
        K_ASSERT(fn != NULL);

        SetMemberFunction(fn, obj);
        SetGPR(4, BitCast<u32>(arg));
        Start();
    }

    /**
     * Member function (const)
     */

    /**
     * @brief Constructor
     *
     * @param fn No-parameter, const member function
     * @param obj Class instance
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*fn)() const, const TClass& obj) {
        K_ASSERT(fn != NULL);

        SetMemberFunction(fn, obj);
        Start();
    }

    /**
     * @brief Constructor
     *
     * @param fn Single-parameter, const member function
     * @param obj Class instance
     * @param arg Function argument
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*fn)(Param) const, const TClass& obj, Param arg) {
        K_ASSERT(fn != NULL);

        SetMemberFunction(fn, obj);
        SetGPR(4, BitCast<u32>(arg));
        Start();
    }

    /**
     * @brief Block the main thread until this thread finishes executing
     */
    void Join() {
        ThreadImpl::Join();
    }
};

namespace detail {

/**
 * @brief Pointer-to-member-function (PTMF)
 */
struct MemberFunction {
    s32 toff; // 'This' pointer offset for target object type
    s32 voff; // Vtable offset into class
    union {
        s32 foff;   // Function offset into vtable
        void* addr; // Raw function address (if voff is -1)
    };
};

/**
 * @brief Set a member function to run on this thread
 *
 * @param fn Function
 * @param obj Class instance
 */
template <typename TFunc, typename TClass>
void ThreadImpl::SetMemberFunction(TFunc fn, const TClass& obj)
    __attribute__((never_inline)) {
    K_STATIC_ASSERT_EX(sizeof(TFunc) == sizeof(MemberFunction),
                       "Not a member function");

    register const MemberFunction* ptmf;
    register u32 self;

    // clang-format off
    asm volatile {
        mr ptmf, r4 // fn  -> ptmf
        mr self, r5 // obj -> self
    }
    K_ASSERT(ptmf != NULL);
    K_ASSERT(self != NULL);
    // clang-format on

    K_ASSERT(mpOSThread != NULL);
    K_ASSERT(mpOSThread->state == OS_THREAD_STATE_READY);

    // Adjust this pointer
    self += ptmf->toff;
    SetGPR(3, self);

    // Non-virtual function?
    if (ptmf->voff == -1) {
        SetFunction(ptmf->addr);
        return;
    }

    // Find virtual function table
    const void** vt = BitCast<const void**>(self + ptmf->voff);

    // Find virtual function address
    K_ASSERT(ptmf->foff >= 0);
    SetFunction(vt[ptmf->foff / sizeof(void*)]);
}

} // namespace detail
} // namespace kiwi

#endif
