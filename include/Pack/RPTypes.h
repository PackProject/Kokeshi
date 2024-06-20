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
    static void createInstance();                                              \
    static void destroyInstance();                                             \
    static T* getInstance() {                                                  \
        return spInstance;                                                     \
    }                                                                          \
                                                                               \
private:                                                                       \
    static T* spInstance;

//! Implementation for a singleton class which uses the current heap
#define RP_SINGLETON_DEF(T)                                                    \
    void T::createInstance() {                                                 \
        spInstance = new T();                                                  \
    }                                                                          \
    void T::destroyInstance() {                                                \
        delete spInstance;                                                     \
        spInstance = NULL;                                                     \
    }

//! @}

//! @name Singleton with specified heap
//! @{

//! Declarations for a singleton class which uses the specified heap
#define RP_SINGLETON_DECL_EX(T)                                                \
public:                                                                        \
    static T* createInstance(EGG::Heap* pHeap);                                \
    static void destroyInstance();                                             \
    static T* getInstance() {                                                  \
        return spInstance;                                                     \
    }                                                                          \
                                                                               \
private:                                                                       \
    static T* spInstance;

//! Implementation for a singleton class which uses the specified heap
#define RP_SINGLETON_DEF_EX(T)                                                 \
    void T::createInstance(EGG::Heap* pHeap) {                                 \
        if (spInstance == NULL) {                                              \
            spInstance = new (pHeap) T(pHeap);                                 \
            return spInstance;                                                 \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    void T::destroyInstance() {                                                \
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
