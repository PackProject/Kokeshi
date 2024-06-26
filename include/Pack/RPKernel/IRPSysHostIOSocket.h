#ifndef RP_KERNEL_I_HOST_IO_SOCKET_H
#define RP_KERNEL_I_HOST_IO_SOCKET_H
#include <Pack/RPTypes.h>

//! @addtogroup rp_kernel
//! @{

/**
 * @brief Host I/O for debugging
 */
class IRPSysHostIOSocket {
public:
    /**
     * @brief Constructor
     * @stubbed
     *
     * @param pName Socket name
     * @param ... Format string arguments
     */
    IRPSysHostIOSocket(const char* pName, ...);
    /**
     * @brief Constructor
     * @stubbed
     */
    IRPSysHostIOSocket();
    /**
     * @brief Destructor
     * @stubbed
     */
    virtual ~IRPSysHostIOSocket(); // at 0x8

    /**
     * @brief Receives UI control values from the host
     * @stubbed
     */
    virtual void ListenPropertyEvent() {} // at 0xC
    /**
     * @brief Sends UI control layout to the host
     * @stubbed
     */
    virtual void GenMessage() {} // at 0x10
};

//! @}

#endif
