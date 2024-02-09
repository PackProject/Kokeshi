#ifndef LIBKIWI_H
#define LIBKIWI_H
#include <libkiwi/core/kiwiColor.hpp>
#include <libkiwi/core/kiwiController.hpp>
#include <libkiwi/core/kiwiDvdStream.hpp>
#include <libkiwi/core/kiwiIStream.hpp>
#include <libkiwi/core/kiwiMemStream.hpp>
#include <libkiwi/core/kiwiNandStream.hpp>
#include <libkiwi/core/kiwiSceneHookMgr.hpp>
#include <libkiwi/kernel/kiwiAssert.h>
#include <libkiwi/kernel/kiwiMemoryMgr.hpp>
#include <libkiwi/kernel/kiwiRuntime.h>
#include <libkiwi/math/kiwiAlgorithm.hpp>
#include <libkiwi/net/kiwiAsyncClient.hpp>
#include <libkiwi/net/kiwiAsyncSocket.hpp>
#include <libkiwi/net/kiwiPacket.hpp>
#include <libkiwi/net/kiwiSocketBase.hpp>
#include <libkiwi/net/kiwiSyncSocket.hpp>
#include <libkiwi/prim/kiwiArray.hpp>
#include <libkiwi/prim/kiwiLinkList.hpp>
#include <libkiwi/prim/kiwiSTL.hpp>
#include <libkiwi/prim/kiwiString.hpp>
#include <libkiwi/rvl/kiwiLibGX.hpp>
#include <libkiwi/rvl/kiwiLibSO.hpp>
#include <libkiwi/util/kiwiAutoLock.hpp>
#include <libkiwi/util/kiwiBuildTarget.hpp>
#include <libkiwi/util/kiwiChecksum.hpp>
#include <libkiwi/util/kiwiDynamicSingleton.hpp>
#include <libkiwi/util/kiwiMapFile.hpp>
#include <libkiwi/util/kiwiNonCopyable.hpp>
#include <libkiwi/util/kiwiNw4rConsole.hpp>
#include <libkiwi/util/kiwiNw4rDirectPrint.hpp>
#include <libkiwi/util/kiwiNw4rException.hpp>
#include <libkiwi/util/kiwiOverride.hpp>
#include <libkiwi/util/kiwiRandom.hpp>
#include <libkiwi/util/kiwiStaticSingleton.hpp>
#include <types.h>

void* operator new(std::size_t size);
void* operator new[](std::size_t size);

void* operator new(std::size_t size, s32 align);
void* operator new[](std::size_t size, s32 align);

void operator delete(void* block);
void operator delete[](void* block);
#endif
