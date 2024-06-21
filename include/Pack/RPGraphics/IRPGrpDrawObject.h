#ifndef RP_GRAPHICS_I_DRAW_OBJECT_H
#define RP_GRAPHICS_I_DRAW_OBJECT_H
#include <Pack/RPTypes.h>

//! @addtogroup rp_graphics
//! @{

/**
 * @brief Interface for renderable objects
 * @wfuname
 *
 * @details The active renderer (RPGrpRenderer) manages a list of these draw
 * objects.
 */
class IRPGrpDrawObject {
private:
    //! Next renderable in the linked list
    IRPGrpDrawObject* mpNext; // at 0x0

public:
    /**
     * @brief Constructor
     */
    IRPGrpDrawObject() : mpNext(NULL) {}
    /**
     * @brief Destructor
     */
    virtual ~IRPGrpDrawObject() {} // at 0x4

    /**
     * @brief Standard draw pass
     */
    virtual void UserDraw() {} // at 0x8
    /**
     * @brief Additional draw pass for debug builds
     */
    virtual void DebugDraw() {} // at 0xC
};

//! @}

#endif
