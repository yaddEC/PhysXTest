#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameObject.h"
#include "Light.h"
#include "Mesh.h"
#include "MyMaths.h"
#include "MonoBehaviour.h"
#include "Collision.h"
#include "Collider.h"
#include "ResourceManager.h"

constexpr auto MAX_DIR_LIGHTS = 2;
constexpr auto MAX_POINT_LIGHTS = 10;
constexpr auto MAX_SPOT_LIGHTS = 2;

namespace LowRenderer
{
	static class Renderer
	{
	public:
		inline static std::vector<PointLight*>		 currentPointLights;
		inline static std::vector<DirectionalLight*> currentDirectionalLights;
		inline static std::vector<SpotLight*>		 currentSpotLights;
		inline static std::vector<Mesh*>			 skybox;

		inline static Vector3* currentViewPos = nullptr;
		inline static Matrix4* currentVP      = nullptr;
		inline static Matrix4* currentView    = nullptr;
		inline static Matrix4* currentProj    = nullptr;

		static void RenderMesh(Mesh* mesh);
		static void RenderReflection(Mesh* mesh);
		static void RenderCubeMap(int skyboxID);
		static void RenderColliders();

	private:
		struct SendToShader
		{
			void SetInt  (int shaderProgram, const char* name, int value);
			void SetFloat(int shaderProgram, const char* name, float value);
			void SetVec3 (int shaderProgram, const char* name, Vector3 value);
			void SetVec4 (int shaderProgram, const char* name, Vector4 value);
			void SetMat4 (int shaderProgram, const char* name, Matrix4 value);
		};
		inline static SendToShader shader;
	};
}