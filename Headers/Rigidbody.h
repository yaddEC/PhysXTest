#pragma once
#include "MyMaths.h"
#include "MonoBehaviour.h"
#include <PhysXManager.h>

using namespace Core::Maths;
using namespace LowRenderer;

namespace Physics
{
	class Rigidbody : public MonoBehaviour
	{
	public:
		Rigidbody();
		~Rigidbody() {};
		void Init();
		void Update() override;
		bool UpdateComponent(std::string* id) override;
		void Delete() override {};

		Vector3 gravity = Vector3(0, -20, 0);
		Vector3 velocity;
		bool isStatic = false;
		float mass = 1;
		bool reset = false;

		PxRigidActor* PhysxActor;
		
	};
}