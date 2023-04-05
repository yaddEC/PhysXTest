#include "Rigidbody.h"
#include "GameObject.h"
#include "PlayerMovements.h"
#include "AppState.h"
#include "Collider.h"
#include <Debug.h>

using namespace Core;
using namespace Game;

Physics::Rigidbody::Rigidbody()
{
}

void Physics::Rigidbody::Init()
{
	PlayerMovements* pm = gameObject->GetComponent<PlayerMovements>();
	if (pm)
		pm->rb = this;
	Collider* c = gameObject->GetComponent<Collider>();
	if (c)
		c->rb = this;
}

void Physics::Rigidbody::Update()
{
	if (AppState::sceneMode == AppState::Mode::EDITOR)
		return;

	if (PhysxActor )
	{
		// Get the updated PxTransform from the PhysX simulation
		
		

		if (!isStatic && PhysxActor && PhysxActor->is<PxRigidDynamic>())
		{
			PxRigidDynamic* dynamicActor = PhysxActor->is<PxRigidDynamic>();

			// Set the updated velocity
			PxVec3 force = PxVec3(velocity.x, velocity.y, velocity.z) * mass;

			// Apply the force to the dynamic actor
			dynamicActor->addForce(force);

			// Update the GameObject's transform
			PxTransform updatedTransform = dynamicActor->getGlobalPose();
			Vector3 newPosition = Vector3(updatedTransform.p.x, updatedTransform.p.y, updatedTransform.p.z);
			gameObject->transform.position = newPosition;

		}

		
		

		// Update the GameObject's transform with the new position and rotation
	
		
	}
	//velocity += AppState::deltatime * gravity;
}

bool Physics::Rigidbody::UpdateComponent(std::string* id)
{
	bool opened = ImGui::CollapsingHeader(("Rigidbody##" + *id).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
	bool popUp = false;
	if (ImGui::IsItemClicked(1))
	{
		popUp = true;
	}
	if (opened)
	{
		ImGui::Text("Velocity\n");		
		ImGui::DragFloat3(("##velo" + *id).c_str(), &velocity.x, 0.1, 0);
		ImGui::Text("Gravity\n");
		ImGui::DragFloat(("##grav" + *id).c_str(), &gravity.y, 0.1, -10, 10);
	}
	return popUp;
}
