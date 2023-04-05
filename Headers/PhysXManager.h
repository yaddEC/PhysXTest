#pragma once

#include <Physx/PxPhysicsAPI.h>
#include <memory>

using namespace physx;

class MySimulationEventCallback : public PxSimulationEventCallback
{
public:

    MySimulationEventCallback();

    ~MySimulationEventCallback();

    void onContact (const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override ;

    void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

    void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)override {};
    void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)override {};
    void onWake(PxActor** actors, PxU32 count) override {};
    void onSleep(PxActor** actors, PxU32 count)override {};



};

enum MaterialType {
    ROCK,
    BOUNCY_BALL,
    ICE,
    RUBBER,
    WOOD,
    METAL,
    GLASS
};

class PhysXManager
{
public:
    PhysXManager();
    ~PhysXManager();

    void Init();
    void Update(float deltaTime);
    void Cleanup();
    PxPhysics* getPhysics() const {return mPhysics;}
    PxScene* getScene() const { return mScene; }
    PxMaterial* createMaterialByType(const PxPhysics* const& physics, MaterialType type);

private:
    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    PxFoundation* mFoundation;
    PxPhysics* mPhysics;
    PxScene* mScene;
    PxPvd* mPvd;

    void CreateFoundation();
    void CreatePhysics();
    void CreateScene();
    void SetupVisualDebugger();
};