#ifndef LIBKIWI_CORE_THREAD_H
#define LIBKIWI_CORE_THREAD_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiBitCast.h>
#include <revolution/OS.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{
namespace detail {

/**
 * @brief Common thread implementation
 */
class ThreadImpl {
public:
    /**
     * @brief Waits for this thread to finish executing
     */
    void Join();

protected:
    /**
     * @brief Constructor
     */
    ThreadImpl();

    /**
     * @brief Destructor
     */
    ~ThreadImpl();

    /**
     * @brief Begins execution on this thread
     */
    void Start();

    /**
     * @brief Sets a function for this thread to run
     *
     * @param pAddr Function address (new SRR0 value)
     */
    void SetFunction(const void* pAddr);
    /**
     * @brief Sets a GPR's value in this thread
     *
     * @param i GPR number
     * @param value New value
     */
    void SetGPR(u32 i, u32 value);

    /**
     * @brief Sets a member function to run on this thread
     *
     * @param pFunc Function
     * @param rObj Class instance
     */
    template <typename TFunc, typename TClass>
    void SetMemberFunction(TFunc pFunc, const TClass& rObj);

private:
    OSThread* mpOSThread; // RVL thread
    u8* mpThreadStack;    // RVL thread stack

    // Thread stack size
    static const u32 scStackSize = 0x4000;
    // Thread priority
    static const s32 scPriority = OS_PRIORITY_MAX / 2;
};

} // namespace detail

/**
 * @brief Similar to std::thread
 * @note Only allows GPR arguments
 */
class Thread : public detail::ThreadImpl {
public:
    // Thread function parameter
    typedef void* Param;

public:
    /**
     * @brief Non-member function
     */

    /**
     * @brief Constructor
     *
     * @param pFunc Static, no-parameter function
     */
    template <typename TRet> Thread(TRet (*pFunc)()) {
        K_ASSERT(pFunc != nullptr);

        SetFunction(pFunc);
        Start();
    }

    /**
     * @brief Constructor
     *
     * @param pFunc Static, single-parameter function
     * @param pArg Function argument
     */
    template <typename TRet> Thread(TRet (*pFunc)(Param), Param pArg) {
        K_ASSERT(pFunc != nullptr);

        SetFunction(pFunc);
        SetGPR(3, BitCast<u32>(pArg));
        Start();
    }

    /**
     * @brief Member function (non-const)
     */

    /**
     * @brief Constructor
     *
     * @param pFunc No-parameter member function
     * @param rObj Class instance
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)(), TClass& rObj) {
        K_ASSERT(pFunc);

        SetMemberFunction(pFunc, rObj);
        Start();
    }

    /**
     * @brief Constructor
     *
     * @param pFunc Single-parameter member function
     * @param rObj Class instance
     * @param pArg Function argument
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)(Param), TClass& rObj, Param pArg) {
        K_ASSERT(pFunc);

        SetMemberFunction(pFunc, rObj);
        SetGPR(4, BitCast<u32>(pArg));
        Start();
    }

    /**
     * @brief Member function (const)
     */

    /**
     * @brief Constructor
     *
     * @param pFunc No-parameter, const member function
     * @param rObj Class instance
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)() const, const TClass& rObj) {
        K_ASSERT(pFunc);

        SetMemberFunction(pFunc, rObj);
        Start();
    }

    /**
     * @brief Constructor
     *
     * @param pFunc Single-parameter, const member function
     * @param rObj Class instance
     * @param pArg Function argument
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)(Param) const, const TClass& rObj, Param pArg) {
        K_ASSERT(pFunc);

        SetMemberFunction(pFunc, rObj);
        SetGPR(4, BitCast<u32>(pArg));
        Start();
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
        s32 foff;    // Function offset into vtable
        void* pAddr; // Raw function address (if voff is -1)
    };
};

/**
 * @brief Sets a member function to run on this thread
 *
 * @param pFunc Function
 * @param rObj Class instance
 */
template <typename TFunc, typename TClass>
K_DONT_INLINE void ThreadImpl::SetMemberFunction(TFunc pFunc,
                                                 const TClass& rObj) {
    K_STATIC_ASSERT_EX(sizeof(TFunc) == sizeof(MemberFunction),
                       "Not a member function");

    register const MemberFunction* pPtmf;
    register u32 self;

    // clang-format off
    asm volatile {
        mr pPtmf, r4 // pFunc -> pPtmf
        mr self, r5 // rObj   -> self
    }
    K_ASSERT(pPtmf != nullptr);
    K_ASSERT(self != 0);
    // clang-format on

    K_ASSERT(mpOSThread != nullptr);
    K_ASSERT(mpOSThread->state == OS_THREAD_STATE_READY);

    // Adjust this pointer
    self += pPtmf->toff;
    SetGPR(3, self);

    // Non-virtual function?
    if (pPtmf->voff == -1) {
        SetFunction(pPtmf->pAddr);
        return;
    }

    // Find virtual function table
    const void** pVtbl = BitCast<const void**>(self + pPtmf->voff);

    // Find virtual function address
    K_ASSERT(pPtmf->foff >= 0);
    SetFunction(pVtbl[pPtmf->foff / sizeof(void*)]);
}

} // namespace detail
//! @}
} // namespace kiwi

#endif
