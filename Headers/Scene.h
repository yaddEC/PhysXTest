#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include  <list>
#include "DevCamera.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Light.h"
#include "ResourceManager.h"
#include "Graph.h"
#include "Collider.h"
#include "Collision.h"
#include "Camera.h"
#include "AppState.h"

#include "PlayerMovements.h"
#include "HeadRotation.h"
#include "Enemy.h"
#include "Rigidbody.h"

#include <imgui_internal.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

constexpr auto MAX_POINTLIGHTS = 4;



using namespace Core;
using namespace LowRenderer;
using namespace Core::Maths;
using namespace DataStructure;
using namespace Physics;
using namespace Game;

namespace Resources
{
	class SceneManager;

	enum class StateMenu
	{
		PLAY,
		MAINMENU,
		EDITOR,
		OPTION,
	};

	class Scene
	{
	public:
		Scene() {};
		~Scene() 
		{
			ResourceManager::DeleteMap();
		};

		void Init(int generatePlatformer);
		void Update(Resources::SceneManager& sceneManager);
		void GeneratePlatformerScene();
		void GenerateLightScene();
	public:
		bool relaod = false;
		int eraseGO = 0;
		int eraseSG = 0;

		Transform objectsTransform;

		DevCamera camera;

		Camera* gameCamera = nullptr;

		int currentCubeMap = 0;

		std::list<GameObject> gameObjects;
		std::list<Graph> sceneGraph;

		std::string name = "";
	private:
		void LoadResources();
		void SwitchMode(AppState::Mode mode);

		// Updates

		void UpdateInspector();
		void UpdateHierarchy();
		void UpdateImGuiGraph(std::list<Graph>& nodeList);
		void UpdateGameObjects(std::list<Graph>& nodeList);

		// Add/Delete GameObjects

		void AddEmptyObject(std::string name, Graph* parent, Transform transform = Transform());
		void AddEmptyObject(std::string name, Transform transform = Transform());
		void AddModelObject(std::string modelName, std::string textureName = "cubeTexture", std::string shaderName = "mainShader", Graph* parent = nullptr, Transform transform = Transform(), Type meshType = Type::NORMAL);
		void AddSkybox(std::string modelName, std::string textureName = "skyBox", std::string shaderName = "skyBoxShader", Transform transform = Transform());

		void AddPointLightObject(Vector3 position = Vector3(), Vector3 color = Vector3(1, 1, 1), Graph* parent = nullptr);
		void AddDirLightObject(Vector3 rotation = Vector3(), Vector3 color = Vector3(1, 1, 1), Graph* parent = nullptr);
		void AddSpotLightObject(Vector3 position = Vector3(), Vector3 rotation = Vector3(0, -1, 0), float angle = 12.0f, Vector3 color = Vector3(1, 1, 1), Graph* parent = nullptr);
		void DeleteGameObject(Graph& node);
		void ResetID() { id = 0; }

		// Add Components 

		void AddMesh(GameObject* object, std::string modelName, Type meshType = Type::NORMAL , std::string textureName = "cubeTexture", std::string shaderName = "mainShader");
		Mesh* AddMesh(std::string modelName, std::string textureName = "cubeTexture", std::string shaderName = "mainShader");
		void AddPointLight(GameObject* object, Vector3 color = Vector3(1, 1, 1));
		void AddDirLight(GameObject* object, Vector3 color = Vector3(1, 1, 1));
		void AddSpotLight(GameObject* object, float angle = 12.0f, Vector3 color = Vector3(1, 1, 1));
		void AddCamera(GameObject* object);
		template <class T>
		void AddCollider(GameObject* object, Vector3 center = Vector3(), Vector3 size = Vector3(1, 1, 1), bool trigger = false);
		template <class T>
		void AddScript(GameObject* object);
		void AddRigidbody(GameObject* object);

		ImGuiTreeNodeFlags GetTreeFlags(Graph& node);

		void SetButtonInOptionImgui(std::string name, Input::Button moveChange);
		
	private:
		GameObject* selectedObject = nullptr;

		StateMenu stateMenu = StateMenu::MAINMENU;
		Input::Button buttonSelected = Input::Button::NULLBUTTON;

		unsigned int id = 0;

		Graph* WorldNode = nullptr;
		bool newcam = false;

		int idScene;
	};
}