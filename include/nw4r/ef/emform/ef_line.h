#ifndef NW4R_EF_EMFORM_LINE_H
#define NW4R_EF_EMFORM_LINE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ef/ef_emitterform.h>

namespace nw4r {
namespace ef {

class EmitterFormLine : public EmitterForm {
public:
    EmitterFormLine() {}

    virtual void Emission(Emitter* pEmitter, ParticleManager* pManager,
                          int count, u32 flags, f32* pParams, u16 life,
                          f32 lifeRnd, const math::MTX34* pSpace); // at 0x8
};

} // namespace ef
} // namespace nw4r

#endif
