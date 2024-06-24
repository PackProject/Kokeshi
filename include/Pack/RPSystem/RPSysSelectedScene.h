#ifndef RP_SYSTEM_SELECTED_SCENE_H
#define RP_SYSTEM_SELECTED_SCENE_H
#include <Pack/RPTypes.h>
#include <egg/core.h>

//! @addtogroup rp_system
//! @{

/**
 * @brief Records scene chosen from the main menu
 */
class RPSysSelectedScene {
    RP_SINGLETON_DECL_EX(RPSysSelectedScene);

public:
    /**
     * @brief Clears the selected scene ID
     * @details The ID is set to -1.
     */
    void reset();

    /**
     * @brief Gets the ID of the selected scene
     * @note The value is -1 if no scene has been selected yet.
     */
    s32 getSceneID() const {
        return mSceneID;
    }
    /**
     * @brief Sets the ID of the selected scene
     *
     * @param id Scene ID
     */
    void setSceneID(s32 id) {
        mSceneID = id;
    }

private:
    //! ID of the scene that was selected from the main menu
    s32 mSceneID; // at 0x8
};

//! @}

#endif
