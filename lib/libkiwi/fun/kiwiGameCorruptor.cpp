#include <cmath>
#include <libkiwi.h>
#include <revolution/OS.h>

namespace kiwi {
namespace {

/**
 * @brief Selects a random address in the specified range
 *
 * @param pBegin Beginning of range
 * @param pEnd End of range
 */
void** GetRandomAddr(const void* pBegin, const void* pEnd) {
    K_ASSERT(pBegin != nullptr);
    K_ASSERT(pEnd != nullptr);
    K_ASSERT(pEnd > pBegin);

    u32 size = GetOffsetFromPtr(pBegin, pEnd);
    u32 offset = 0;

    do {
        // Make sure to align the address to 4 bytes
        offset = Random().NextU32(size);
        offset = ROUND_UP(offset, 4);
    }
    // Adjusting for alignment may take us out of the range
    while (offset >= size);

    return const_cast<void**>(AddToPtr<void*>(pBegin, offset));
}

} // namespace

K_DYNAMIC_SINGLETON_IMPL(GameCorruptor);

/**
 * @brief Constructor
 */
GameCorruptor::GameCorruptor()
    : mDomainFlag(ECorruptDomain_Mem2),
      mNumCorrupt(scDefaultNum),
      mInterval(OS_SEC_TO_TICKS(scDefaultInterval)) {}

/**
 * @brief Destructor
 */
GameCorruptor::~GameCorruptor() {
    OSCancelAlarm(&mAlarm);
}

/**
 * @brief Corruption alarm handler
 *
 * @param pAlarm OS alarm
 * @param pCtx Alarm context
 */
void GameCorruptor::AlarmHandler(OSAlarm* pAlarm, OSContext* pCtx) {
#pragma unused(pAlarm)
#pragma unused(pCtx)

    GetInstance().Corrupt();
}

/**
 * @brief Performs one corruption cycle
 */
void GameCorruptor::Corrupt() const {
    ECorruptDomain flag =
        static_cast<ECorruptDomain>(BitUtil::RandomBit(mDomainFlag));

    switch (flag) {
    case ECorruptDomain_DolCode:
        CorruptCode(GetDolTextStart(), GetDolTextEnd());
        break;

    case ECorruptDomain_DolData:
        // 50/50 between .data and .rodata
        if (Random().CoinFlip()) {
            CorruptData(GetDolDataStart(), GetDolDataEnd());
        } else {
            CorruptData(GetDolRodataStart(), GetDolRodataEnd());
        }
        break;

    case ECorruptDomain_Mem1:
        CorruptData(RP_GET_INSTANCE(RPSysSystem)->getRootHeapMem1());
        break;

    case ECorruptDomain_Mem2:
        CorruptData(RP_GET_INSTANCE(RPSysSystem)->getRootHeapMem2());
        break;

    case ECorruptDomain_Scene: CorruptData(EGG::Heap::getCurrentHeap()); break;

    default: K_ASSERT_EX(false, "Invalid corrupt flag"); break;
    }
}

/**
 * @brief Corrupts some code instructions in the specified range
 *
 * @param pBegin Beginning of range
 * @param pEnd End of range
 */
void GameCorruptor::CorruptCode(const void* pBegin, const void* pEnd) const {
    K_LOG_EX("CorruptCode %08X-%08X\n", pBegin, pEnd);
}

/**
 * @brief Corrupts some pieces of data in the specified range
 *
 * @param pBegin Beginning of range
 * @param pEnd End of range
 */
void GameCorruptor::CorruptData(const void* pBegin, const void* pEnd) const {
    K_LOG_EX("CorruptData %08X-%08X\n", pBegin, pEnd);

    for (int i = 0; i < mNumCorrupt;) {
        void** addr = GetRandomAddr(pBegin, pEnd);

        // Don't corrupt libkiwi
        if (PtrUtil::IsLibKiwi(addr)) {
            continue;
        }

        // Try to avoid strings (may break filepaths)
        if (PtrUtil::IsString(addr)) {
            continue;
        }

        // Don't touch pointers
        if (PtrUtil::IsPointer(*addr) || *addr == 0) {
            continue;
        }
        if (PtrUtil::IsPtmf(addr)) {
            K_LOG_EX("PTMF? %08X\n", addr);
            continue;
        }

        // May corrupt a heap block tag
        if (PtrUtil::IsMBlockTag(addr) ||
            PtrUtil::IsMBlockTag(AddToPtr(addr, sizeof(u16)))) {
            K_LOG_EX("Heap tag? %08X\n", addr);
            continue;
        }

        // Corrupt float
        if (PtrUtil::IsFloat(addr)) {
            *reinterpret_cast<f32*>(addr) =
                Random().NextF32(10000000.0f) * Random().Sign();
        }
        // Corrupt integer
        else {
            *reinterpret_cast<u32*>(addr) = Random().NextU32();
        }

        i++;
    }
}

/**
 * @brief Corrupts some pieces of data in the specified heap
 *
 * @param pHeap Heap to corrupt
 */
void GameCorruptor::CorruptData(EGG::Heap* pHeap) const {
    K_ASSERT(pHeap != nullptr);
    CorruptData(pHeap->getStartAddress(), pHeap->getEndAddress());
}

} // namespace kiwi
