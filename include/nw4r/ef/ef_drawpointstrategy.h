#ifndef NW4R_EF_DRAW_POINT_STRATEGY_H
#define NW4R_EF_DRAW_POINT_STRATEGY_H
#include "ef_drawstrategyimpl.h"
#include "types_nw4r.h"

namespace nw4r {
namespace ef {
class DrawPointStrategy : public DrawStrategyImpl {
public:
    DrawPointStrategy();
    virtual ~DrawPointStrategy() {} // at 0x8

    void Draw(const DrawInfo&, ParticleManager*);
    void InitGraphics(const DrawInfo&, ParticleManager*);
};
} // namespace ef
} // namespace nw4r

#endif
