#ifndef NW4R_EF_DRAW_FREE_STRATEGY_H
#define NW4R_EF_DRAW_FREE_STRATEGY_H
#include "ef_drawstrategyimpl.h"
#include "types_nw4r.h"

namespace nw4r {
namespace ef {
class DrawFreeStrategy : public DrawStrategyImpl {
public:
    DrawFreeStrategy();
    virtual ~DrawFreeStrategy() {} // at 0x8

    UNKTYPE Draw(const DrawInfo&, ParticleManager*);
};
} // namespace ef
} // namespace nw4r

#endif
