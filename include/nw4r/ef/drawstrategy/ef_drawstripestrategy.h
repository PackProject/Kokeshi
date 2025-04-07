#ifndef NW4R_EF_DRAW_STRIPE_STRATEGY_H
#define NW4R_EF_DRAW_STRIPE_STRATEGY_H
#include "ef_drawstrategyimpl.h"
#include "types_nw4r.h"

namespace nw4r {
namespace ef {
class DrawStripeStrategy : public DrawStrategyImpl {
public:
    DrawStripeStrategy();
    virtual UNKTYPE GetCalcAheadFunc(ParticleManager*); // at 0x18

    virtual UNKTYPE Draw(const DrawInfo&, ParticleManager*);
};
} // namespace ef
} // namespace nw4r

#endif
