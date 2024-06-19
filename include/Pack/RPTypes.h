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
    static T* GetInstance() {                                                  \
        return sInstance;                                                      \
    }                                                                          \
                                                                               \
private:                                                                       \
    static T* sInstance;

//! Implementation for a singleton class which uses the current heap
#define RP_SINGLETON_DEF(T)                                                    \
    void T::CreateInstance() {                                                 \
        sInstance = new T();                                                   \
    }                                                                          \
    void T::DestroyInstance() {                                                \
        delete sInstance;                                                      \
        sInstance = NULL;                                                      \
    }

//! @}

//! @name Singleton with specified heap
//! @{

//! Declarations for a singleton class which uses the specified heap
#define RP_SINGLETON_DECL_EX(T)                                                \
public:                                                                        \
    static T* CreateInstance(EGG::Heap* pHeap);                                \
    static void DestroyInstance();                                             \
    static T* GetInstance() {                                                  \
        return sInstance;                                                      \
    }                                                                          \
                                                                               \
private:                                                                       \
    static T* sInstance;

//! Implementation for a singleton class which uses the specified heap
#define RP_SINGLETON_DEF_EX(T)                                                 \
    void T::CreateInstance(EGG::Heap* pHeap) {                                 \
        if (sInstance == NULL) {                                               \
            sInstance = new (pHeap) T();                                       \
            return sInstance;                                                  \
        }                                                                      \
        return NULL;                                                           \
    }                                                                          \
    void T::DestroyInstance() {                                                \
        delete sInstance;                                                      \
        sInstance = NULL;                                                      \
    }

//! @}

//! Accesses RP singleton instance by typename
#ifndef RP_GET_INSTANCE
#define RP_GET_INSTANCE(T) T::GetInstance()
#endif

//! Holds year, month, month day
typedef u16 RPTime16;
//! Holds year, month, month day, week day, hour
typedef u32 RPTime32;

//! @}

#endif
