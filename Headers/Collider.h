#pragma once
#include "MyMaths.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include <PhysXManager.h>
#include "Rigidbody.h"
#include <vector>

using namespace Core::Maths;
using namespace LowRenderer;

namespace Physics
{
	class Collider : public MonoBehaviour
	{
	public:
		virtual Vector3 FindFurthestPoint(Vector3 direction) = 0;
		virtual void ShowCollider(int program) = 0;
		virtual void setShape(PxMaterial& material) = 0;

		bool show = false;
		bool isTrigger = false;
		bool collide = false;
		Vector3 center;
		Rigidbody* rb;
		PxShape* shape;
		PxRigidActor* PhysxActor;

		void Init(PxPhysics& physics, PxMaterial& material, PxScene& scene);

		void Update() override;
		void Delete() override;

		

	};

	class BoxCollider : public Collider
	{
	public:
		BoxCollider(Vector3 _size = Vector3(1, 1, 1));
		bool UpdateComponent(std::string* id) override;
		Vector3 FindFurthestPoint(Vector3 direction) override;
		void ShowCollider(int program) override;
		void setShape(PxMaterial& material) override;
		Vector3 size;
		std::vector<Vector3> vertices;
		PxBoxGeometry geometry;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(float _radius = 1);
		bool UpdateComponent(std::string* id) override;
		Vector3 FindFurthestPoint(Vector3 direction) override;
		void ShowCollider(int program) override;
		void setShape(PxMaterial& material) override;
		float radius;
		float scaledRadius;
		PxSphereGeometry geometry;
	};

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider(float _radius = 1, float _height = 1);
		bool UpdateComponent(std::string* id) override;
		Vector3 FindFurthestPoint(Vector3 direction) override;
		void ShowCollider(int program) override;
		void setShape(PxMaterial& material) override;
		float radius;
		float height;

		float scale = 0;
		float hScale = 0;
		Vector3 worldScale = Vector3(1, 1, 1);

		void InitCapsule();
		std::vector<Vector3> vertices;
		PxCapsuleGeometry geometry;
	};
}
