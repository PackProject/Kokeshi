#ifndef RP_SYSTEM_COMMON_OBJECT_H
#define RP_SYSTEM_COMMON_OBJECT_H
#include <Pack/RPGraphics/IRPGrpDrawObject.h>
#include <Pack/RPTypes.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Encapsulates common object behavior/rendering
 * @wfuname
 *
 * @details Encapsulates scene fader/effects, pause manager, system/tutorial
 * windows, etc.
 * @note Appends itself to the renderer list for automatic rendering.
 */
class RPSysCommonObject : public IRPGrpDrawObject {
public:
    /**
     * @brief Constructor
     */
    RPSysCommonObject();
    /**
     * @brief Destructor
     */
    virtual ~RPSysCommonObject() {}

    /**
     * @brief Initializes common objects' state
     */
    void init();

    /**
     * @brief Updates common objects' state before the scene gets to update
     */
    void preUpdate();
    /**
     * @brief Updates common objects' state after the scene gets to update
     */
    void postUpdate();

    /**
     * @brief Renders common objects' state
     */
    virtual void UserDraw();
};

//! @}

#endif
