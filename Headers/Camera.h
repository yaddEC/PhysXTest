#pragma once
#include "GameObject.h"
#include "MonoBehaviour.h"
#include <vector>
#include "MyMaths.h"
#include "Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace Core::Maths;

namespace LowRenderer
{
	class Camera : public MonoBehaviour
	{
	public:
		Matrix4 VP;
		Matrix4 ViewMatrix;
		Matrix4 ProjectionMatrix;
	public:

		Camera() {};
		~Camera() {};
		void Init(unsigned int width, unsigned int height);
		void Update() override;
		bool UpdateComponent(std::string* id) override;
		void Delete() override {};

	private:

	};
}
