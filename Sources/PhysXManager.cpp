#include <PhysXManager.h>
#include <iostream>

PxDefaultErrorCallback PhysXManager::gDefaultErrorCallback;
PxDefaultAllocator PhysXManager::gDefaultAllocatorCallback;

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
    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
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