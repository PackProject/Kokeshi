#ifndef LIBKIWI_UTIL_SEQUENCE_H
#define LIBKIWI_UTIL_SEQUENCE_H
#include <libkiwi/kernel/kiwiAssert.h>
#include <types.h>

namespace kiwi {

/**
 * Scene phase
 *
 * @tparam T Scene type
 */
template <typename T> class Phase {
public:
    /**
     * Destructor
     */
    virtual ~Phase() {}

    /**
     * Enter callback
     *
     * @param param User parameter
     */
    virtual void Enter(T* param) {}

    /**
     * Calculate callback
     *
     * @param param User parameter
     */
    virtual void Calculate(T* param) {}

    /**
     * UserDraw callback
     *
     * @param param User parameter
     */
    virtual void UserDraw(T* param) {}

    /**
     * DebugDraw callback
     *
     * @param param User parameter
     */
    virtual void DebugDraw(T* param) {}

    /**
     * Exit callback
     *
     * @param param User parameter
     */
    virtual void Exit(T* param) {}
};

/**
 * Scene phase manager
 *
 * @tparam T Scene type
 */
template <typename T> class Sequence {
public:
    /**
     * Constructor
     *
     * @param param Phase user parameter
     */
    Sequence(T* param) : mpPhase(NULL), mpParam(param) {}

    /**
     * Destructor
     */
    virtual ~Sequence() {}

    /**
     * Change scene to new phase
     *
     * @param phase New phase
     */
    void ChangePhase(Phase<T>* phase) {
        // Outgoing
        if (mpPhase != NULL) {
            mpPhase->Exit(mpParam);
        }

        // Incoming
        if (phase != NULL) {
            phase->Enter(mpParam);
        }

        mpPhase = phase;
    }

    /**
     * Update current phase
     */
    void Calculate() {
        if (mpPhase != NULL) {
            mpPhase->Calculate(mpParam);
        }
    }

    /**
     * Draw current phase (user level)
     */
    void UserDraw() {
        if (mpPhase != NULL) {
            mpPhase->UserDraw(mpParam);
        }
    }

    /**
     * Draw current phase (debug level)
     */
    void DebugDraw() {
#ifndef LIBKIWI_RELEASE
        if (mpPhase != NULL) {
            mpPhase->DebugDraw(mpParam);
        }
#endif
    }

private:
    // Current scene phase
    Phase<T>* mpPhase;
    // Phase user parameter
    T* mpParam;
};

} // namespace kiwi

#endif
