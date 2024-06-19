#ifndef RP_UTILITY_BASE_FSM_H
#define RP_UTILITY_BASE_FSM_H
#include "RPTypes.h"

#include <climits>

/**
 * @brief Declare enter/calc functions by state name
 */
#define RP_UTL_FSM_STATE_DECL(X)                                               \
    void State_##X##_enter();                                                  \
    void State_##X##_calc();

/**
 * @brief Generic state machine
 */
template <typename T> class RPUtlBaseFsm {
public:
    /**
     * @brief State member function
     */
    typedef void (T::*StateFunc)();

public:
    /**
     * @brief Constructor
     *
     * @param object Object for member functions
     * @param numState Max number of states
     */
    RPUtlBaseFsm(T* object, int numState)
        : mObject(object), mNumState(numState) {
        mEnterFns = new StateFunc[mNumState];
        mCalcFns = new StateFunc[mNumState];
        mExitFns = new StateFunc[mNumState];
    }

    /**
     * @brief Register new state with this machine
     *
     * @param enter State enter function
     * @param calc State update function
     * @param id State ID
     */
    void RegistState(StateFunc enter, StateFunc calc, u32 id) {
        mEnterFns[id] = enter;
        mCalcFns[id] = calc;
    }

    /**
     * @brief Change to new state
     */
    void ChangeState(u32 id) {
        mNextState = id;
        mDoEnter = true;
        mDoExit = true;
    }

    /**
     * @brief Run state machine logic
     */
    void Calculate() {
        if (mDoEnter) {
            if (mDoExit) {
                ExitState();
            }

            mPrevState = mState;
            mState = mNextState;

            mDoEnter = false;
            mDuration = 0;
            EnterState();
        }

        CalcState();

        // Prevent overflow
        if (mDuration < INT_MAX) {
            mDuration++;
        }
    }

    /**
     * @brief Check for a specific state
     */
    bool IsState(u32 id) const {
        // Currently in the state
        if (mState == id) {
            return true;
        }

        // Entering the state on next tick
        if ((mDoEnter || mDoExit) && mNextState == id) {
            return true;
        }

        return false;
    }

private:
    /**
     * @brief Enter the current state
     */
    void EnterState() {
        if (mEnterFns[mState] != NULL) {
            (mObject->*mEnterFns[mState])();
        }
    }

    /**
     * @brief Update the current state
     */
    void CalcState() {
        if (mCalcFns[mState] != NULL) {
            (mObject->*mCalcFns[mState])();
        }
    }

    /**
     * @brief Exit the current state
     */
    void ExitState() {
        if (mExitFns[mState] != NULL) {
            (mObject->*mExitFns[mState])();
        }
    }

private:
    // Current state ID
    int mState; // at 0x0
    // State ID before change
    int mPrevState; // at 0x4
    // State ID after change
    int mNextState; // at 0x8

    // Duration of current state
    int mDuration; // at 0xC

    // Call enter function on state change
    bool mDoEnter; // at 0x10
    // Call exit function on state change
    bool mDoExit; // at 0x11

    // Object for member functions
    T* mObject; // at 0x14

    // State enter functions
    StateFunc* mEnterFns; // at 0x18
    // State update functions
    StateFunc* mCalcFns; // at 0x1C
    // State exit functions
    StateFunc* mExitFns; // at 0x20

    // Max number of states
    int mNumState; // at 0x24
};

#endif
