#include <PhysXManager.h>
#include <iostream>

PxDefaultErrorCallback PhysXManager::gDefaultErrorCallback;
PxDefaultAllocator PhysXManager::gDefaultAllocatorCallback;


MySimulationEventCallback::MySimulationEventCallback()
{
}

MySimulationEventCallback::~MySimulationEventCallback()
{
}

void MySimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{

    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];

        // only interested in contacts found with the shapes of the collider
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            printf("ENTER COLLISION\n");

        }

        // only interested in contacts lost with the shapes of the collider
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            //  printf("EXIT\n");

        }

        //  printf("STAY\n");

    }
}
void MySimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    for (PxU32 i = 0; i < count; i++)
    {
        PxTriggerPair& pair = pairs[i];
        PxShape* triggerShape = pair.triggerShape;

        // check if the other shape is a trigger or not
        bool isOtherTrigger = pair.otherShape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE;

        // ENTER
        if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND && !isOtherTrigger)
        {
            printf("ENTER TRIGGER\n");
        }

        // EXIT
        if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST && !isOtherTrigger)
        {
             printf("EXIT TRIGGER\n");
        }

        printf("STAY TRIGGER\n");

    }

}

PhysXManager::PhysXManager()
    : mFoundation(nullptr),
    mPhysics(nullptr),
    mScene(nullptr),
    mPvd(nullptr)
{
}

PhysXManager::~PhysXManager()
{
    Cleanup();
}

void PhysXManager::Init()
{
    CreateFoundation();
    SetupVisualDebugger();
    CreatePhysics();
    CreateScene();
}

void PhysXManager::Update(float deltaTime)
{
    if (mScene)
    {
        mScene->simulate(deltaTime);
        mScene->fetchResults(true);
    }
}

void PhysXManager::Cleanup()
{
    if (mScene)
        mScene->release();

    if (mPhysics)
        mPhysics->release();

    if (mPvd)
        mPvd->release();

    if (mFoundation)
        mFoundation->release();
}

PxMaterial* PhysXManager::createMaterialByType(const PxPhysics* const& physics, MaterialType type)
{
    PxReal staticFriction, dynamicFriction, restitution;

    switch (type) {
    case ROCK:
        staticFriction = 0.8f;
        dynamicFriction = 0.7f;
        restitution = 0.1f;
        break;
    case BOUNCY_BALL:
        staticFriction = 0.5f;
        dynamicFriction = 0.3f;
        restitution = 0.8f;
        break;
    case ICE:
        staticFriction = 0.1f;
        dynamicFriction = 0.05f;
        restitution = 0.2f;
        break;
    case RUBBER:
        staticFriction = 0.7f;
        dynamicFriction = 0.5f;
        restitution = 0.6f;
        break;
    case WOOD:
        staticFriction = 0.6f;
        dynamicFriction = 0.4f;
        restitution = 0.3f;
        break;
    case METAL:
        staticFriction = 0.4f;
        dynamicFriction = 0.2f;
        restitution = 0.1f;
        break;
    case GLASS:
        staticFriction = 0.6f;
        dynamicFriction = 0.5f;
        restitution = 0.2f;
        break;
    default:
       
        break;
    }

    return mPhysics->createMaterial(staticFriction, dynamicFriction, restitution);
}


void PhysXManager::CreateFoundation()
{
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (!mFoundation)
        throw std::runtime_error("PxCreateFoundation failed!");
}

void PhysXManager::CreatePhysics()
{
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);
    if (!mPhysics)
        throw std::runtime_error("PxCreatePhysics failed!");
}

void PhysXManager::CreateScene()
{
    MySimulationEventCallback* mySimulationEventCallback = new MySimulationEventCallback();
    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    sceneDesc.simulationEventCallback = mySimulationEventCallback;
    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
        throw std::runtime_error("createScene failed!");
}

void PhysXManager::SetupVisualDebugger()
{
    
        mPvd = PxCreatePvd(*mFoundation);
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    
}