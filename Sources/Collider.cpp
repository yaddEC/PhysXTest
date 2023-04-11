#include <glad/glad.h>
#include "Collider.h"
#include "ResourceManager.h"
#include "Renderer.h"

void Physics::Collider::Init(PxPhysics& physics, PxMaterial& material, PxScene& scene)
{
	Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
	if (rigidbody)
		rb = rigidbody;
	Matrix4 worldModel = gameObject->GetWorldModel();
	PxVec3 positionPointer = PxVec3(worldModel.matrix[0][3] + center.x, worldModel.matrix[1][3] + center.y, worldModel.matrix[2][3] + center.z);
	
	PxTransform pose(positionPointer);

	if (rb->isStatic) {
		rb->PhysxActor = physics.createRigidStatic(pose);
	}
	else {
		rb->PhysxActor = physics.createRigidDynamic(pose);
		rb->PhysxActor->is<PxRigidDynamic>()->setMass(rb->mass);
	}
	setShape(material);

	scene.addActor(*rb->PhysxActor);
}

void Physics::Collider::Update()
{
	
	/*Matrix4 worldModel = gameObject->GetWorldModel();
	PxVec3 position(worldModel.matrix[0][3] + center.x, worldModel.matrix[1][3] + center.y, worldModel.matrix[2][3] + center.z);
	PxTransform pose(position);
	rb->PhysxActor->setGlobalPose(pose);*/
}

void Physics::Collider::Delete()
{
	
}

Physics::BoxCollider::BoxCollider(Vector3 _size)
{
	size = _size;
	Vector3 seg = size / 2.0f;
	geometry = PxBoxGeometry(seg.x, seg.y, seg.z);
	vertices.push_back(Vector3(-seg.x, -seg.y, seg.z));
	vertices.push_back(Vector3(seg.x, -seg.y, seg.z));
	vertices.push_back(Vector3(seg.x, seg.y, seg.z));
	vertices.push_back(Vector3(-seg.x, seg.y, seg.z));

	vertices.push_back(Vector3(-seg.x, -seg.y, -seg.z));
	vertices.push_back(Vector3(seg.x, -seg.y, -seg.z));
	vertices.push_back(Vector3(seg.x, seg.y, -seg.z));
	vertices.push_back(Vector3(-seg.x, seg.y, -seg.z));
}

void Physics::BoxCollider::ShowCollider(int program)
{
	Matrix4 newMVP = gameObject->MVP * Matrix4::CreateTranslationMatrix(center) * Matrix4::CreateScaleMatrix(size);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, newMVP.ptr());
	int isActive = active;
	int colliding = collide;
	glUniform1iv(glGetUniformLocation(program, "isActive"), 1, &isActive);
	glUniform1iv(glGetUniformLocation(program, "collide"), 1, &colliding);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(ResourceManager::Get<Model>("myCube")->GetVAO());
	glDrawElements(GL_TRIANGLES, ResourceManager::Get<Model>("myCube")->indexBuffer.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Physics::BoxCollider::setShape(PxMaterial& material)
{
	shape = PxRigidActorExt::createExclusiveShape(*rb->PhysxActor, geometry, material);

	

	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

bool Physics::BoxCollider::UpdateComponent(std::string* id)
{
	bool opened = ImGui::CollapsingHeader(("BoxCollider##" + *id).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
	bool popUp = false;
	if (ImGui::IsItemClicked(1))
	{
		popUp = true;
	}
	if (opened)
	{
		ImGui::Checkbox(("active##" + *id).c_str(), &active);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Checkbox(("show##" + *id).c_str(), &show);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Checkbox(("Is Trigger##" + *id).c_str(), &isTrigger);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Text("\Cnenter");
		ImGui::DragFloat3(("##center" + *id).c_str(), &center.x, 0.1, -100, 100);
		ImGui::Text("\Size");
		ImGui::DragFloat3(("##size" + *id).c_str(), &size.x, 0.01, 0.01, 100);
	}
	return popUp;
}

Vector3 Physics::BoxCollider::FindFurthestPoint(Vector3 direction) 
{

	Vector3 maxPoint;
	float   maxDistance = -99999999;

	for (Vector3 vertex : vertices) 
	{
		Vector3 globalVertex = (gameObject->GetWorldModel() * Matrix4::CreateTranslationMatrix(center) * Matrix4::CreateScaleMatrix(size) * Vector4(vertex, 1)).xyz;
		float distance = globalVertex.DotProduct(direction);
		if (distance > maxDistance) 
		{
			maxDistance = distance;
			maxPoint = globalVertex;
		}
	}

	return maxPoint;
}

#pragma region Sphere


Physics::SphereCollider::SphereCollider(float _radius)
{
	radius = _radius;
	geometry = PxSphereGeometry(_radius);
}

void Physics::SphereCollider::ShowCollider(int program)
{
	Vector3 worldScale = Vector3(1, 1, 1);
	Graph* o = gameObject->node;
	while (o != nullptr)
	{
		worldScale *= o->object->transform.scale;
		o = o->parent;
	}
	float scale = worldScale.x;
	if (scale < worldScale.y) scale = worldScale.y;
	if (scale < worldScale.z) scale = worldScale.z;

	scaledRadius = radius * scale;

	Matrix4 newMVP = gameObject->MVP * Matrix4::CreateTranslationMatrix(center) * Matrix4::CreateScaleMatrix(Vector3(scaledRadius / worldScale.x, scaledRadius / worldScale.y, scaledRadius / worldScale.z));

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, newMVP.ptr());
	int isActive = active;
	int colliding = collide;
	glUniform1iv(glGetUniformLocation(program, "isActive"), 1, &isActive);
	glUniform1iv(glGetUniformLocation(program, "collide"), 1, &colliding);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(ResourceManager::Get<Model>("mySphere")->GetVAO());
	glDrawElements(GL_TRIANGLES, ResourceManager::Get<Model>("mySphere")->indexBuffer.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Physics::SphereCollider::setShape(PxMaterial& material)
{
	shape = PxRigidActorExt::createExclusiveShape(*rb->PhysxActor, geometry, material);
	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

}

bool Physics::SphereCollider::UpdateComponent(std::string* id)
{
	bool opened = ImGui::CollapsingHeader(("SphereCollider##" + *id).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
	bool popUp = false;
	if (ImGui::IsItemClicked(1))
	{
		popUp = true;
	}
	if (opened)
	{
		ImGui::Checkbox(("active##" + *id).c_str(), &active);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Checkbox(("show##" + *id).c_str(), &show);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Checkbox(("Is Trigger##" + *id).c_str(), &isTrigger);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Text("\Cnenter");
		ImGui::DragFloat3(("##center" + *id).c_str(), &center.x, 0.1, -100, 100);
		ImGui::Text("\nRadius");
		ImGui::DragFloat(("##radius" + *id).c_str(), &radius, 0.01, 0.001, 100);
	}
	return popUp;
}

Vector3 Physics::SphereCollider::FindFurthestPoint(Vector3 direction) 
{
	direction = direction.Normalize();

	Vector3 worldScale = Vector3(1, 1, 1);
	Graph* o = gameObject->node;
	while (o != nullptr)
	{
		worldScale *= o->object->transform.scale;
		o = o->parent;
	}
	float scale = worldScale.x;
	if (scale < worldScale.y) scale = worldScale.y;
	if (scale < worldScale.z) scale = worldScale.z;

	scaledRadius = radius * scale;

	Vector3 pos = (gameObject->GetWorldModel() * Vector4(center, 1)).xyz;
	Vector3 maxPoint = (scaledRadius * direction) + pos;
	return maxPoint;
}
#pragma endregion

#pragma region Capsule


Physics::CapsuleCollider::CapsuleCollider(float _radius, float _height)
{
	radius = _radius;
	height = _height;
	geometry = PxCapsuleGeometry(_radius, _height *0.5f);
	InitCapsule();
}

void Physics::CapsuleCollider::ShowCollider(int program)
{
	worldScale = Vector3(1, 1, 1);
	Graph* o = gameObject->node;
	while (o != nullptr)
	{
		worldScale *= o->object->transform.scale;
		o = o->parent;
	}
	scale = worldScale.x;
	if (scale < worldScale.z) scale = worldScale.z;
	hScale = worldScale.y;

	Matrix4 newMVP = gameObject->MVP * Matrix4::CreateTranslationMatrix(center) * Matrix4::CreateScaleMatrix(Vector3(radius / worldScale.x * scale, radius / worldScale.y * scale, height / worldScale.z * hScale));

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, newMVP.ptr());
	int isActive = active;
	int colliding = collide;
	glUniform1iv(glGetUniformLocation(program, "isActive"), 1, &isActive);
	glUniform1iv(glGetUniformLocation(program, "collide"), 1, &colliding);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(ResourceManager::Get<Model>("myCapsule")->GetVAO());
	glDrawElements(GL_TRIANGLES, ResourceManager::Get<Model>("myCapsule")->indexBuffer.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Physics::CapsuleCollider::setShape(PxMaterial& material)
{
	shape = PxRigidActorExt::createExclusiveShape(*rb->PhysxActor, geometry, material);
	if (isTrigger)
	{
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

bool Physics::CapsuleCollider::UpdateComponent(std::string* id)
{
	bool opened = ImGui::CollapsingHeader(("CapsuleCollider##" + *id).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
	bool popUp = false;
	if (ImGui::IsItemClicked(1))
	{
		popUp = true;
	}
	if (opened)
	{
		ImGui::Checkbox(("active##" + *id).c_str(), &active);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Checkbox(("show##" + *id).c_str(), &show);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Checkbox(("Is Trigger##" + *id).c_str(), &isTrigger);
		ImGui::Dummy(ImVec2(10, 10));
		ImGui::Text("\Cnenter");
		ImGui::DragFloat3(("##center" + *id).c_str(), &center.x, 0.1, -100, 100);
		ImGui::Text("\nRadius");
		ImGui::DragFloat(("##radius" + *id).c_str(), &radius, 0.01, 0.01, 100);
		ImGui::Text("\nHeight");
		ImGui::DragFloat(("##height" + *id).c_str(), &height, 0.01, 0.01, 100);
	}
	return popUp;
}

void Physics::CapsuleCollider::InitCapsule()
{
	Model* capsule = ResourceManager::Get<Model>("myCapsule");
	
	for (int i = 0; i < capsule->vertexBuffer.size(); i++)
	{
		bool duplicate = false;
		for (int j = 0; j < vertices.size(); j++)
		{
			if (vertices[j] == capsule->vertexBuffer[i].position)
			{
				duplicate = true;
				continue;
			}
		}
		if(duplicate) continue;
		vertices.push_back(capsule->vertexBuffer[i].position);
	}
}

Vector3 Physics::CapsuleCollider::FindFurthestPoint(Vector3 direction) 
{

	Vector3 maxPoint;
	float   maxDistance = -99999999;

	for (Vector3 vertex : vertices)
	{
		Vector3 globalVertex = (gameObject->GetWorldModel() * Matrix4::CreateTranslationMatrix(center) * Matrix4::CreateScaleMatrix(Vector3(radius / worldScale.x * scale, radius / worldScale.y * scale, height / worldScale.z * hScale)) * Vector4(vertex, 1)).xyz;
		float distance = globalVertex.DotProduct(direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = globalVertex;
		}
	}

	return maxPoint;
}
#pragma endregion
