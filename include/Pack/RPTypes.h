#ifndef RP_TYPES_H
#define RP_TYPES_H
#include <egg/core.h>
#include <egg/prim.h>
#include <types.h>

//! @addtogroup rp
//! @{

//! @name Standard singleton
//! @{

//! Declarations for a singleton which uses the current heap
#define RP_SINGLETON_DECL(T)                                                   \
public:                                                                        \
    static void CreateInstance();                                              \
    static void DestroyInstance();                                             \
    static T* getInstance() {                                                  \
        return spInstance;                                                     \
    };                                                                         \
                                                                               \
private:                                                                       \
    T();                                                                       \
    virtual ~T();                                                              \
                                                                               \
private:                                                                       \
    /** Class singleton instance */                                            \
    static T* spInstance;

//! Implementation for a singleton class which uses the current heap
#define RP_SINGLETON_DEF(T)                                                    \
    void T::CreateInstance() {                                                 \
        spInstance = new T();                                                  \
    }                                                                          \
    void T::DestroyInstance() {                                                \
        delete spInstance;                                                     \
        spInstance = NULL;                                                     \
    }

//! @}

//! @name Singleton with specified heap
//! @{

//! Declarations for a singleton class which uses the specified heap
#define RP_SINGLETON_DECL_EX(T)                                                \
public:                                                                        \
    static T* CreateInstance(EGG::Heap* pHeap);                                \
    static void DestroyInstance();                                             \
    static T* getInstance() {                                                  \
        return spInstance;                                                     \
    }                                                                          \
                                                                               \
private:                                                                       \
    T(EGG::Heap* pHeap);                                                       \
    virtual ~T();                                                              \
                                                                               \
private:                                                                       \
    /** Heap in which this object was allocated */                             \
    EGG::Heap* mpParentHeap;                                                   \
                                                                               \
private:                                                                       \
    /** Class singleton instance */                                            \
    static T* spInstance;

//! Implementation for a singleton class which uses the specified heap
#define RP_SINGLETON_DEF_EX(T)                                                 \
    T* T::spInstance = NULL;                                                   \
                                                                               \
    void T::CreateInstance(EGG::Heap* pHeap) {                                 \
        if (spInstance == NULL) {                                              \
            spInstance = new (pHeap) T(pHeap);                                 \
            return spInstance;                                                 \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    void T::DestroyInstance() {                                                \
        delete spInstance;                                                     \
        spInstance = NULL;                                                     \
    }

//! @}

//! Accesses RP singleton instance by typename
#ifndef RP_GET_INSTANCE
#define RP_GET_INSTANCE(T) T::getInstance()
#endif

//! @brief Holds year, month, month day
//! @note Only years after (and including) 2000 can be represented.
typedef u16 RPTime16;

//! @brief Holds year, month, month day, week day, hour
//! @note Only years after (and including) 2000 can be represented.
typedef u32 RPTime32;

//! @}

#endif
