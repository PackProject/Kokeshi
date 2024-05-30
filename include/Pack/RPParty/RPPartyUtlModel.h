#ifndef RP_PARTY_UTL_MODEL_H
#define RP_PARTY_UTL_MODEL_H
#include <Pack/RPGraphics/IRPGrpDrawObject.h>
#include <Pack/RPKernel/IRPSysHostIOSocket.h>
#include <Pack/RPSystem/RP_DEBUG_STUB_1.h>
#include <egg/math/eggVector.h>
#include <types.h>

// Forward declarations
class RPGrpModel;

/**
 * @brief Party Pack model utility
 */
class RPPartyUtlModel : public RP_DEBUG_STUB_1,
                        public IRPSysHostIOSocket,
                        public IRPGrpDrawObject {
public:
    RPPartyUtlModel();
    virtual ~RPPartyUtlModel();

    virtual const EGG::Vector3f& getPosition() const;
    virtual void setPosition(const EGG::Vector3f& pos);

    virtual const EGG::Vector3f& getVelocity() const;
    virtual void setVelocity(const EGG::Vector3f& vel);

    virtual const EGG::Vector3f& FUN_801b8ff0() const;
    virtual void FUN_801b8ff8(const EGG::Vector3f& v);

    virtual const EGG::Vector3f& getMtxZBase() const;
    virtual void setMtxZBase(const EGG::Vector3f& z);

    virtual const EGG::Vector3f& getMtxYBase() const;
    virtual void setMtxYBase(const EGG::Vector3f& y);

    virtual const EGG::Vector3f& getMtxXBase() const;
    virtual void setMtxXBase(const EGG::Vector3f& x);

    virtual f32 getScale() const;
    virtual void setScale(f32 scale);

    virtual RPGrpModel* getModel() const;
    virtual void setModel(RPGrpModel* model);
    virtual void calcModel();

private:
    EGG::Vector3f mPosition; // at 0x10
    EGG::Vector3f mVelocity; // at 0x1C
    EGG::Vector3f _28;       // at 0x28

    EGG::Vector3f mMtxZBase; // at 0x34
    EGG::Vector3f mMtxYBase; // at 0x40
    EGG::Vector3f mMtxXBase; // at 0x4C

    f32 mScale;          // at 0x58
    RPGrpModel* mpModel; // at 0x5C
};

#endif
