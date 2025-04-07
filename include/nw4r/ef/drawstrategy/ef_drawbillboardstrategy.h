#ifndef NW4R_EF_DRAW_BILLBOARD_STRATEGY_H
#define NW4R_EF_DRAW_BILLBOARD_STRATEGY_H
#include "ef_drawstrategyimpl.h"
#include "types_nw4r.h"

namespace nw4r {
namespace ef {
class DrawBillboardStrategy : public DrawStrategyImpl {
public:
    DrawBillboardStrategy();
    virtual UNKTYPE GetCalcAheadFunc(ParticleManager*); // at 0x18

    virtual void Draw(const DrawInfo&, ParticleManager*);
};
} // namespace ef
} // namespace nw4r

#endif
