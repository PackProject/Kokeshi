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
//! @addtogroup libkiwi_core
//! @{

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
    OSThread* mpOSThread; //!< RVL thread
    u8* mpThreadStack;    //!< RVL thread stack

    static const u32 scStackSize = 0x4000;             //!< Thread stack size
    static const s32 scPriority = OS_PRIORITY_MAX / 2; //!< Thread priority
};

//! @}
} // namespace detail

/**
 * @brief Run function on another thread
 * @details Similar to std::thread.
 * @note Only allows GPR arguments
 */
class Thread : public detail::ThreadImpl {
public:
    // Thread function parameter
    typedef void* Param;

public:
    /**
     * @name Non-member function
     */
    /**@{*/
    /**
     * @brief Constructor
     *
     * @param pFunc Static, no-parameter function
     */
    template <typename TRet> Thread(TRet (*pFunc)());
    /**
     * @brief Constructor
     *
     * @param pFunc Static, single-parameter function
     * @param pArg Function argument
     */
    template <typename TRet> Thread(TRet (*pFunc)(Param), Param pArg);
    /**@}*/

    /**
     * @name Member function (non-const)
     */
    /**@{*/
    /**
     * @brief Constructor
     *
     * @param pFunc No-parameter member function
     * @param rObj Class instance
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)(), TClass& rObj);
    /**
     * @brief Constructor
     *
     * @param pFunc Single-parameter member function
     * @param rObj Class instance
     * @param pArg Function argument
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)(Param), TClass& rObj, Param pArg);
    /**@}*/

    /**
     * @name Member function (const)
     */
    /**@{*/
    /**
     * @brief Constructor
     *
     * @param pFunc No-parameter, const member function
     * @param rObj Class instance
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)() const, const TClass& rObj);
    /**
     * @brief Constructor
     *
     * @param pFunc Single-parameter, const member function
     * @param rObj Class instance
     * @param pArg Function argument
     */
    template <typename TRet, typename TClass>
    Thread(TRet (TClass::*pFunc)(Param) const, const TClass& rObj, Param pArg);
    /**@}*/
};

//! @}
} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_CORE_THREAD_IMPL_HPP
#include <libkiwi/core/kiwiThreadImpl.hpp>
#endif

#endif
