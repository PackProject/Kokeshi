#ifndef LIBKIWI_UTIL_EXTENSION_H
#define LIBKIWI_UTIL_EXTENSION_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiNonConstructable.h>

namespace kiwi {
//! @addtogroup libkiwi_util
//! @{

/**
 * @brief Extend class using only pre-existing instances
 */
template <typename TBase>
class Extension : public TBase, private NonConstructable {};

/**
 * @brief Extend singleton class (pointer) using only pre-existing instances
 */
template <typename TBase, typename TDerived>
class ExtSingletonPtr : public TBase, private NonConstructable {
public:
    /**
     * @brief Accesses singleton instance (by pointer)
     */
    static TDerived& GetInstance() {
        K_ASSERT(TBase::getInstance() != nullptr);
        return static_cast<TDerived&>(*TBase::getInstance());
    }
};

/**
 * @brief Extend singleton class (reference) using only pre-existing instances
 */
template <typename TBase, typename TDerived>
class ExtSingletonRef : public TBase, private NonConstructable {
public:
    /**
     * @brief Accesses singleton instance (by reference)
     */
    static TDerived& GetInstance() {
        return static_cast<TDerived&>(TBase::getInstance());
    }
};

//! @}
} // namespace kiwi

#endif
