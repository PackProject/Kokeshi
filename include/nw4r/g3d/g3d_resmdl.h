#ifndef NW4R_G3D_RESMDL_H
#define NW4R_G3D_RESMDL_H
#include "g3d_rescommon.h"
#include "g3d_resdict.h"
#include "g3d_resnode.h"
#include "types_nw4r.h"

namespace nw4r {
namespace g3d {
struct ResMdlData {
    char kind[4];       // "MDL0"
    u32 size;           // at 0x4
    u32 revision;       // at 0x8
    s32 resFileOfs;     // at 0xC
    u32 byteCodeOfs;    // at 0x10
    u32 nodeOfs;        // at 0x14
    u32 vtxPosOfs;      // at 0x18
    u32 vtxNrmOfs;      // at 0x1C
    u32 vtxClrOfs;      // at 0x20
    u32 vtxTxcOfs;      // at 0x24
    u32 matOfs;         // at 0x28
    u32 tevOfs;         // at 0x2C
    u32 shpOfs;         // at 0x30
    u32 plttTexInfoOfs; // at 0x34
};

struct ResMdl {
    enum { REVISION = 9 };

    ResCommon<ResMdlData> mMdl;

    inline ResMdl(void* vptr) : mMdl(vptr) {}
    bool IsValid() const {
        return mMdl.IsValid();
    }

    u8* GetResByteCode(const char*) const;

    ResNode GetResNode(const char*) const;
    ResNode GetResNode(ResName) const;
    ResNode GetResNode(int) const;
    ResNode GetResNode(u32) const;
    u32 GetResNodeNumEntries() const;

    ResVtxPos GetResVtxPos(ResName) const;
    ResVtxPos GetResVtxPos(int) const;
    ResVtxPos GetResVtxPos(u32) const;
    u32 GetResVtxPosNumEntries() const;

    ResVtxNrm GetResVtxNrm(ResName) const;
    ResVtxNrm GetResVtxNrm(int) const;
    ResVtxNrm GetResVtxNrm(u32) const;
    u32 GetResVtxNrmNumEntries() const;

    ResVtxClr GetResVtxClr(ResName) const;
    ResVtxClr GetResVtxClr(int) const;
    ResVtxClr GetResVtxClr(u32) const;
    u32 GetResVtxClrNumEntries() const;

    ResVtxTexCoord GetResVtxTexCoord(int) const;

    ResMat GetResMat(const char*) const;
    ResMat GetResMat(ResName) const;
    ResMat GetResMat(int) const;
    ResMat GetResMat(u32) const;
    u32 GetResMatNumEntries() const;

    ResShp GetResShp(const char*) const;
    ResShp GetResShp(int) const;
    ResShp GetResShp(u32) const;
    u32 GetResShpNumEntries() const;

    ResTexPlttInfo GetResTexPlttInfoOffsetFromTexName(int) const;
    u32 GetResTexPlttInfoOffsetFromTexNameNumEntries() const;

    bool Bind(ResFile);
    void Release();
    void Init();
    void Terminate();

    inline bool CheckRevision() const {
        return mMdl.ref().revision == REVISION;
    }

    inline u32 GetResVtxTexCoordNumEntries() const {
        ResMdlData& ref = mMdl.ref();
        return mMdl.ofs_to_obj<ResDic>(ref.vtxTxcOfs).GetNumData();
    }
};
} // namespace g3d
} // namespace nw4r

#endif
