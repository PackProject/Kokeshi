#ifndef NW4R_EF_DRAW_SMOOTH_STRIPE_STRATEGY_H
#define NW4R_EF_DRAW_SMOOTH_STRIPE_STRATEGY_H
#include "ef_drawstrategyimpl.h"
#include "types_nw4r.h"

namespace nw4r {
namespace ef {
class DrawSmoothStripeStrategy : public DrawStrategyImpl {
public:
    DrawSmoothStripeStrategy();
    virtual ~DrawSmoothStripeStrategy() {}              // at 0x8
    virtual UNKTYPE GetCalcAheadFunc(ParticleManager*); // at 0x18

    UNKTYPE Draw(const DrawInfo&, ParticleManager*);
};
} // namespace ef
} // namespace nw4r

#endif
