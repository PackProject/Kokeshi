#ifndef LIBKIWI_PRIM_SMART_PTR_H
#define LIBKIWI_PRIM_SMART_PTR_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * @brief "Smart" / scoped pointer (equivalent to std::unique_ptr)
 */
template <typename T> class SmartPtr {
public:
    /**
     * @brief Constructor
     *
     * @details Empty pointer
     */
    SmartPtr() : mpData(NULL) {}

    /**
     * @brief Constructor
     *
     * @param ptr Pointer
     */
    SmartPtr(const T* ptr) : mpData(ptr) {}

    /**
     * @brief Constructor
     *
     * @param ptr Smart pointer
     */
    SmartPtr(SmartPtr& other) : mpData(NULL) {
        *this = other;
    }

    /**
     * @brief Destructor
     */
    ~SmartPtr() {
        Destroy();
    }

    /**
     * @brief Move assignment
     *
     * @param ptr Pointer
     */
    SmartPtr& operator=(const T* ptr) {
        // Release memory for old object
        Destroy();

        // Nothing special required to take ownership
        mpData = ptr;

        return *this;
    }

    /**
     * @brief Move assignment
     *
     * @param other Smart pointer
     */
    SmartPtr& operator=(SmartPtr& other) {
        // Release memory for old object
        Destroy();

        // Move pointer to take ownership
        mpData = other.mpData;
        other.mpData = NULL;

        return *this;
    }

    /**
     * @brief Access held pointer
     */
    T* Get() {
        return mpData;
    }
    /**
     * @brief Access held pointer (read-only)
     */
    const T* Get() const {
        return mpData;
    }

    /**
     * @brief Destroy and release object
     */
    void Destroy() {
        if (mpData != NULL) {
            delete mpData;
            mpData = NULL;
        }
    }

    /**
     * @brief Release object without destroying it
     *
     * @return Pointer to object
     */
    T* Release() const {
        T* old = mpData;
        mpData = NULL;
        return old;
    }

    /**
     * @brief Boolean conversion operator
     */
    operator bool() const {
        return Get() != NULL;
    }

    /**
     * @brief Dereference pointer
     */
    T& operator*() {
        K_ASSERT(mpData != NULL);
        return *Get();
    }
    /**
     * @brief Dereference pointer (read-only)
     */
    const T& operator*() const {
        K_ASSERT(mpData != NULL);
        return *Get();
    }

    /**
     * @brief Dereference pointer
     */
    T& operator->() {
        K_ASSERT(mpData != NULL);
        return *Get();
    }
    /**
     * @brief Dereference pointer (read-only)
     */
    const T& operator->() const {
        K_ASSERT(mpData != NULL);
        return *Get();
    }

private:
    // Do not allow copying smart pointers (const disallows moving)
    SmartPtr(const SmartPtr& other) : mpData(NULL) {
        K_ASSERT(false);
    }
    SmartPtr& operator=(const SmartPtr& other) {
        K_ASSERT(false);
        return *this;
    }

private:
    // Object owned by this smart pointer
    T* mpData;
};

namespace {

/**
 * @brief Smart pointer construction helper
 */
template <typename T> SmartPtr<T> MakeSmartPtr() {
    return SmartPtr<T>(new T());
}

} // namespace
} // namespace kiwi

#endif
