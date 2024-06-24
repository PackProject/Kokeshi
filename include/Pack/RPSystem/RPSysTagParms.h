#ifndef RP_SYSTEM_TAG_PARMS_H
#define RP_SYSTEM_TAG_PARMS_H
#include <Pack/RPTypes.h>
#include <egg/core.h>

// Forward declarations
class RPSysTagParm;

//! @addtogroup rp_system
//! @{

/**
 * @brief Group of tag parameters
 * @wfuname
 */
class RPSysTagParameters {
public:
    /**
     * @brief Constructor
     *
     * @param pName Parameter group name
     */
    RPSysTagParameters(char* pName);

private:
    //! Name of parameter group
    const char* mpName; // at 0x0
    //! Child parameters
    RPSysTagParm* mpParameters; // at 0x4
};

/**
 * @brief Base class for tag parameters
 * @wfuname
 */
class RPSysTagParm {
public:
    /**
     * @brief Constructor
     *
     * @param pParent Parent container
     * @param pTag Parameter tag
     */
    RPSysTagParm(RPSysTagParameters* pParent, char* pTag);

    /**
     * @brief Reads parameter value
     *
     * @param rStream Input stream
     */
    virtual void read(EGG::Stream& rStream);
    /**
     * @brief Writes parameter value
     *
     * @param rStream Output stream
     */
    virtual void write(EGG::Stream& rStream);
    /**
     * @brief Prints parameter value
     * @stubbed
     */
    virtual void dump();

private:
    //! Parameter tag
    char* mpTag; // at 0x0
    //! Next parameter in group
    RPSysTagParm* mpNext; // at 0x4
};

/**
 * @brief Primitive-typed tag parameter
 * @wfuname
 *
 * @tparam Parameter primitive type
 */
template <typename T> class RPSysPrimTagParm : public RPSysTagParm {
public:
    /**
     * @brief Constructor
     *
     * @param pParent Parent container
     * @param pTag Parameter tag
     */
    RPSysPrimTagParm(RPSysTagParameters* pParent, char* pTag)
        : RPSysTagParm(pParent, pTag) {}

    /**
     * @brief Reads parameter value
     *
     * @param rStream Input stream
     */
    virtual void read(EGG::Stream& rStream);
    /**
     * @brief Writes parameter value
     *
     * @param rStream Output stream
     */
    virtual void write(EGG::Stream& rStream);
    /**
     * @brief Prints parameter value
     * @stubbed
     */
    virtual void dump();

private:
    //! Parameter value
    T mValue; // at 0xC
};

/**
 * @brief String-typed tag parameter
 * @wfuname
 */
class RPSysStringTagParm : public RPSysTagParm {
public:
    /**
     * @brief Constructor
     *
     * @param pParent Parent container
     * @param pTag Parameter tag
     */
    RPSysStringTagParm(RPSysTagParameters* pParent, char* pTag);

    /**
     * @brief Reads parameter value
     *
     * @param rStream Input stream
     */
    virtual void read(EGG::Stream& rStream);
    /**
     * @brief Writes parameter value
     *
     * @param rStream Output stream
     */
    virtual void write(EGG::Stream& rStream);
    /**
     * @brief Prints parameter value
     * @stubbed
     */
    virtual void dump();

private:
    //! Parameter value
    const char* mpValue; // at 0xC
};

/**
 * @brief Reads parameter value
 *
 * @param rStream Input stream
 */
template <> inline void RPSysPrimTagParm<int>::read(EGG::Stream& rStream) {
    mValue = rStream.read_s32();
}
/**
 * @brief Writes parameter value
 *
 * @param rStream Output stream
 */
template <> inline void RPSysPrimTagParm<int>::write(EGG::Stream& rStream) {
    rStream.write_s32(mValue);
}
/**
 * @brief Prints parameter value
 * @stubbed
 */
template <> inline void RPSysPrimTagParm<int>::dump() {}

//! @}

#endif
