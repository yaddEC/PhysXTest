#include "Scene.h"
#include "Renderer.h"
#include "Input.h"
#include "SceneManager.h"
#include "RotatingLight.h"

#include <vector>

#include <imgui_internal.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>


namespace Resources {
	//-------------------------------------------------------------------------------------------------------------------------------------
	//                                        MAIN FUNCTIONS
	//-------------------------------------------------------------------------------------------------------------------------------------
	void Scene::Init(int generatePlatformer)
	{

		mPhysXManager.Init();
		PxPhysics* physics = mPhysXManager.getPhysics();
		rockMaterial = mPhysXManager.createMaterialByType(physics, ROCK);

		idScene = generatePlatformer;
		camera.Init(AppState::windowWidth, AppState::windowHeight);

		LoadResources();

		AddEmptyObject("World", nullptr);
		WorldNode = gameObjects.back().node;
		switch (generatePlatformer)
		{
		case 0:
			GeneratePlatformerScene();
			break;
		case 1:
			GenerateLightScene();
			break;
		default:
			break;
		}
		

		if (AppState::sceneMode == AppState::Mode::EDITOR)
		{
			Renderer::currentVP = &camera.VP;
			Renderer::currentProj = &camera.ProjectionMatrix;
			Renderer::currentView = &camera.ViewMatrix;
		}
		else
		{
			glfwSetCursorPos(Input::window, AppState::windowWidth / 2.0f, AppState::windowHeight / 2.0f);
			if (newcam)
			{
				Renderer::currentVP = &gameCamera->VP;
				Renderer::currentProj = &gameCamera->ProjectionMatrix;
				Renderer::currentView = &gameCamera->ViewMatrix;
			}
			else
			{
				Renderer::currentVP = &camera.VP;
				Renderer::currentProj = &camera.ProjectionMatrix;
				Renderer::currentView = &camera.ViewMatrix;
			}
			Input::HideCursor();
		}

		relaod = false;
	}

	void Scene::GeneratePlatformerScene()
	{
		AddSkybox("cube");
		

		// PLAYER

		AddEmptyObject("Player", Transform(Vector3(70, 50, 0), Vector3(0, 0, 0)));
		Graph* playerNode = gameObjects.back().node;
		gameObjects.back().layer = Layer::PLAYER;
		AddRigidbody(playerNode->object);
		AddCollider<SphereCollider>(playerNode->object, *rockMaterial, Vector3(), Vector3(0.5, 0.5, 0.5),false,false);


		AddEmptyObject("Head", playerNode, Transform());
		AddScript<HeadRotation>(&gameObjects.back());

		AddEmptyObject("Camera", gameObjects.back().node, Transform(Transform(Vector3(0, 1, -2.5), Vector3(-15, 180, 0), Vector3(1, 1, 1))));
		AddCamera(&gameObjects.back());

		AddModelObject("boo", "booTexture", "mainShader", playerNode, Transform(Vector3(0, -0.52, -0.055), Vector3(), Vector3(0.012, 0.012, 0.012)));

		Graph* boo = gameObjects.back().node;
		AddModelObject("cube", "cubeTexture", "mainShader", boo, Transform(Vector3(0, -50, 0), Vector3(), Vector3(100, 50, 100)));
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial, Vector3(0, 0, 0), Vector3(1, 1, 1),true,true);

		AddScript<PlayerMovements>(playerNode->object);

		AddEmptyObject("Lights");
		Graph* lightNode = &sceneGraph.back();
		AddSpotLightObject(Vector3(82, 2, 0), Vector3(-90, 0, 0), 40, Vector3(1, 1, 1), &lightNode->children.back());
		AddDirLightObject(Vector3(45, 45, 0), Vector3(1, 1, 1), &lightNode->children.back());
		AddEmptyObject("Point Lights", &lightNode->children.back());
		Graph* pointNode = gameObjects.back().node;
		AddPointLightObject(Vector3(-1, 0, 2), Vector3(0, 0, 1), pointNode);
		AddPointLightObject(Vector3(0, 0, 2), Vector3(1, 1, 1), pointNode);
		AddPointLightObject(Vector3(1, 0, 2), Vector3(1, 0, 0), pointNode);


		//AddModelObject("skyboxTest", "skyboxTexture", "noLightsShader", &sceneGraph.back(), Transform(Vector3(0, -100, 0), Vector3(0, 0, 0), Vector3(1, 1, 1)));

		AddModelObject("myCube", "cubeTexture", "mainShader");
		DeleteGameObject(sceneGraph.back());
		// PLATFORMS

		AddEmptyObject("Platforms", &sceneGraph.back());
		Graph* platformsNode = gameObjects.back().node;
		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(0, -3, 0), Vector3(), Vector3(10, 2, 10)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(12, -3, 0), Vector3(), Vector3(5, 2, 5)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(17, -1, 0), Vector3(), Vector3(5, 2, 5)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(24, -1, 0), Vector3(30, 0, 0), Vector3(5, 2, 5)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(31, -1, 0), Vector3(-30, 0, 0), Vector3(5, 2, 5)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("mySphere", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(38, -1, 0), Vector3(), Vector3(2, 2, 2)));
		
		AddCollider<SphereCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(48, -1, 0), Vector3(0, 0, 30), Vector3(12, 2, 2)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("burger", "burgerTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(60, -5, 0), Vector3(0, 0, 0), Vector3(0.8, 0.8, 0.8)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(68, -3, 0), Vector3(), Vector3(5, 2, 5)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", platformsNode, Transform(Vector3(80, -3, 0), Vector3(), Vector3(10, 2, 10)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("chest", "chestTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(83, -2, 0), Vector3(0, -70, 0), Vector3(0.8, 0.8, 0.8)));
		
		AddEmptyObject("collider", gameObjects.back().node);
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("goomba", "goombaTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(4, -2, 0), Vector3(0, -90, 0), Vector3(0.03, 0.03, 0.03)));
		gameObjects.back().layer = Layer::ENEMY;
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial,Vector3(0,1,0),Vector3(33,33,33));
		AddScript<Enemy>(&gameObjects.back());
		Enemy* enemy = (Enemy*)gameObjects.back().components.back();
		enemy->AddPathPoint(Vector3(4, -2, -2));
		enemy->AddPathPoint(Vector3(4, -2, 2));

		AddModelObject("goomba", "goombaTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(15.25, 0, 1.75), Vector3(0, -90, 0), Vector3(0.03, 0.03, 0.03)));
		gameObjects.back().layer = Layer::ENEMY;
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);
		AddScript<Enemy>(&gameObjects.back());
		enemy = (Enemy*)gameObjects.back().components.back();
		enemy->AddPathPoint(Vector3(15.25, 0, -1.75));
		enemy->AddPathPoint(Vector3(18.75, 0, -1.75));
		enemy->AddPathPoint(Vector3(18.75, 0, 1.75));
		enemy->AddPathPoint(Vector3(15.25, 0, 1.75));

		AddModelObject("goomba", "goombaTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(75.75, -2, 0), Vector3(0, -90, 0), Vector3(0.03, 0.03, 0.03)));
		gameObjects.back().layer = Layer::ENEMY;
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);
		AddScript<Enemy>(&gameObjects.back());
		enemy = (Enemy*)gameObjects.back().components.back();
		enemy->AddPathPoint(Vector3(75.75, -2, 2));
		enemy->AddPathPoint(Vector3(75.75, -2, -2));


		/* AddModelObject("ball","ballTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(-5, -1.2, 4), Vector3(0, 0, 0), Vector3(0.05, 0.05, 0.05)));*/
		AddModelObject("myCapsule", "cubeTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(-3.5, -1.25, 3), Vector3(0, 0, 0), Vector3(0.5, 0.5, 0.5)));
		AddRigidbody(&gameObjects.back());
		AddCollider<CapsuleCollider>(&gameObjects.back(), *rockMaterial);
		AddModelObject("myCapsule", "cubeTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(3.5, -1, 3), Vector3(90, 0, 0), Vector3(0.5, 0.5, 0.5)));
		AddRigidbody(&gameObjects.back());
		AddCollider<CapsuleCollider>(&gameObjects.back(), *rockMaterial);


		AddModelObject("cube", "cubeTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(-3.5, -1, -3.5), Vector3(45, 45, 45), Vector3(1, 1, 1)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial,Vector3(),Vector3(1,1,1),true);

		AddModelObject("cube", "cubeTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(81, -1, 5.5), Vector3(0, 0, 0), Vector3(10, 2.5, 1)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(81, -1, -5.5), Vector3(0, 0, 0), Vector3(10, 2.5, 1)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "cubeTexture", "mainShader", &sceneGraph.back(), Transform(Vector3(85.5, -1, 0), Vector3(0, 0, 0), Vector3(1, 2.5, 10)));
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

		AddModelObject("cube", "skyBox", "reflectionShader", &sceneGraph.back(), Transform(Vector3(0, 0.5, -3), Vector3(0, 0, 0), Vector3(1.5, 1.5, 1.5)), Type::REFLECTION);
		//AddModelObject("cube", "cubeTexture", "blinnPhongShader", reflectionNode, Transform(Vector3(0, 0, -3), Vector3(), Vector3(1, 1, 1)), Type::NORMAL);
		
		AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);

	}

	void Scene::GenerateLightScene()
	{
		name = "Lights";

		AddEmptyObject("Spheres", &sceneGraph.back());
		Graph* spheresNode = gameObjects.back().node;
		
		AddModelObject("mySphere", "cubeTexture", "flatShader", spheresNode, Transform(Vector3(-7.5, 0, -3), Vector3(), Vector3(1, 1, 1)));
		Vector3* pos = &gameObjects.back().transform.position;
		AddPointLightObject(*pos, Vector3(1, 1, 1), &sceneGraph.back().children.back());
		AddScript<RotatingLight>(&gameObjects.back());
		RotatingLight* script = (RotatingLight*)gameObjects.back().components.back();
		script->SetAll(pos, 2, 1);

		AddModelObject("mySphere", "cubeTexture", "gouraudShader", spheresNode, Transform(Vector3(-3, 0, -6), Vector3(), Vector3(1, 1, 1)));
		pos = &gameObjects.back().transform.position;
		AddPointLightObject(*pos, Vector3(1, 1, 1), &sceneGraph.back().children.back());
		AddScript<RotatingLight>(&gameObjects.back());
		script = (RotatingLight*)gameObjects.back().components.back();
		script->SetAll(pos, 2, 1);

		AddModelObject("mySphere", "cubeTexture", "PhongShader", spheresNode, Transform(Vector3(3, 0, -6), Vector3(), Vector3(1, 1, 1)));
		pos = &gameObjects.back().transform.position;
		AddPointLightObject(*pos, Vector3(1, 1, 1), &sceneGraph.back().children.back());
		AddScript<RotatingLight>(&gameObjects.back());
		script = (RotatingLight*)gameObjects.back().components.back();
		script->SetAll(pos, 2, 1);

		AddModelObject("mySphere", "cubeTexture", "blinnPhongShader", spheresNode, Transform(Vector3(7.5, 0, -3), Vector3(), Vector3(1, 1, 1)));
		pos = &gameObjects.back().transform.position;
		AddPointLightObject(*pos, Vector3(1, 1, 1), &sceneGraph.back().children.back());
		AddScript<RotatingLight>(&gameObjects.back());
		script = (RotatingLight*)gameObjects.back().components.back();
		script->SetAll(pos, 2, 1);

	}

	void Scene::Update(SceneManager& sceneManager)
	{


		if (!ResourceManager::GetInstance().timerPrinted && ResourceManager::CheckLoading())
		{
			ResourceManager::GetInstance().loadingTime = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ResourceManager::GetInstance().beginTime).count();
			ResourceManager::GetInstance().timerPrinted = true;
		}
		ResourceManager::TextureBinding();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (AppState::sceneMode == AppState::Mode::EDITOR) // EDITOR MODE
		{
			ImGui::Begin("PlayMode", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::SetWindowPos(ImVec2(AppState::windowWidth / 2.0f - 47.5, -20));
			ImGui::SetWindowSize(ImVec2(100, 110));
			ImGui::Indent();
			ImGui::Dummy(ImVec2(2, 2));
			if (ImGui::Button("PLAY"))
			{
				if (selectedObject)
				{
					selectedObject = nullptr;
				}
				stateMenu = StateMenu::MAINMENU;
				SwitchMode(AppState::Mode::PLAY);
			}
			if (ImGui::Button("RELOAD") && ResourceManager::CheckLoading())
			{
				relaod = true;
				ResourceManager::DeleteMap();
				int size = sceneGraph.size();
				for (int i = 0; i < size; i++)
				{
					DeleteGameObject(sceneGraph.back());
				}
				gameObjects.clear();
				sceneGraph.clear();
				gameCamera = nullptr;
				ResetID();
				ResourceManager::GetInstance().everythingLoaded = false;
				ResourceManager::GetInstance().timerPrinted = false;
				Init(idScene);
			}
			if (ImGui::Button("AVERAGE") && ResourceManager::CheckLoading())
			{
				ResourceManager::GetInstance().averageTime = 0;
				vector<int> timers;
				for (int i = 0; i < 50; i++)
				{
					ResourceManager::GetInstance().everythingLoaded = false;
					ResourceManager::DeleteMap();
					std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();
					LoadResources();
					while (!ResourceManager::CheckLoading()) {}


					timers.push_back(std::chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beginTime).count());
				}
				for (int i = 0; i < timers.size(); i++)
				{
					ResourceManager::GetInstance().averageTime += timers[i];
				}
				ResourceManager::GetInstance().averageTime /= timers.size();
			}
			//if (ImGui::Button("test"))
			//{
			//	Init();
			//}
			ImGui::Unindent();
			ImGui::End();

			ImGui::Begin("Stats", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::SetWindowPos(ImVec2(250, 0));
			ImGui::SetWindowSize(ImVec2(300, 100));
			ImGui::Indent();
			ImGui::Dummy(ImVec2(2, 2));
			ImGui::Checkbox("Multi-Threading", &ResourceManager::GetInstance().mulithreadLoading);
			ImGui::Text("Loading Time: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(ResourceManager::GetInstance().loadingTime).c_str());
			ImGui::SameLine();
			ImGui::Text(" ms");
			ImGui::Text("Average Time (x50): ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(ResourceManager::GetInstance().averageTime).c_str());
			ImGui::SameLine();
			ImGui::Text(" ms");
			ImGui::Unindent();
			ImGui::End();

			if (Input::IsEscapePressed())
				glfwSetWindowShouldClose(Input::window, true);

			Renderer::RenderCubeMap(currentCubeMap);
			if (!relaod)
			{
				camera.Update();
				Renderer::currentViewPos = &camera.position;
				UpdateGameObjects(sceneGraph);
				mPhysXManager.Update(Core::AppState::deltatime);
				Renderer::RenderColliders();
			}

			UpdateHierarchy();
			UpdateInspector();

		}
		else                                               // PLAY MODE
		{
			switch (stateMenu)
			{
			case StateMenu::PLAY:
			{
				Input::HideCursor();
				Vector3 pos;
				if (gameCamera)
				{
					Matrix4 camModel = gameCamera->gameObject->GetWorldModel();
					pos = Vector3(camModel.matrix[0][3], camModel.matrix[1][3], camModel.matrix[2][3]);
				}
				else
				{
					pos = camera.position;
				}
				Renderer::currentViewPos = &pos;
				Renderer::RenderCubeMap(currentCubeMap);
				UpdateGameObjects(sceneGraph);
				mPhysXManager.Update(Core::AppState::deltatime);
				if (Input::showColliders)
					Renderer::RenderColliders();

				if (Input::IsEscapePressed())
				{
					stateMenu = StateMenu::MAINMENU;
				}
				if (Input::IsColliderKeyPressed())
					Input::showColliders = !Input::showColliders;

				break;
			}
			case StateMenu::MAINMENU:
			{
				Input::ShowCursor();
				AppState::sceneMode = AppState::Mode::PLAY;
				ImGui::Begin("Menu", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
				ImGui::SetWindowPos(ImVec2(0, -20));
				ImGui::SetWindowSize((ImVec2(AppState::windowWidth, AppState::windowHeight + 20)));
				ImGui::Text("\n\n\n\n\n\n\n\n");
				ImGui::Indent((AppState::windowWidth / 2) - 45);
				ImGui::Text("Platformer");
				ImGui::Unindent((AppState::windowWidth / 2) - 45);
				ImGui::Indent(AppState::windowWidth / 3);

				ImGui::Text("\n\n\n\n");
				if (ImGui::Button("Start", ImVec2(AppState::windowWidth / 3, 50)))
				{
					stateMenu = StateMenu::PLAY;
				}
				ImGui::Text("\n\n");
				if (ImGui::Button("Editor", ImVec2(AppState::windowWidth / 3, 50)))
				{
					stateMenu = StateMenu::EDITOR;
					SwitchMode(AppState::Mode::EDITOR);
				}
				ImGui::Text("\n\n");
				if (ImGui::Button("Options", ImVec2(AppState::windowWidth / 3, 50)))
				{
					stateMenu = StateMenu::OPTION;
					if (buttonSelected != Input::Button::NULLBUTTON)
						buttonSelected = Input::Button::NULLBUTTON;
				}
				ImGui::Text("\n\n");
				if (ImGui::Button("Change Scene", ImVec2(AppState::windowWidth / 3, 50)))
				{
					ResourceManager::DeleteMap();
					int size = sceneGraph.size();
					for (int i = 0; i < size; i++)
					{
						DeleteGameObject(sceneGraph.back());
					}
					gameObjects.clear();
					sceneGraph.clear();
					mPhysXManager.Cleanup();
					gameCamera = nullptr;
					ResetID();
					ResourceManager::GetInstance().everythingLoaded = false;
					ResourceManager::GetInstance().timerPrinted = false;
					relaod = true;
					sceneManager.currentScene = nullptr;
				}
				ImGui::Text("\n\n");
				if (ImGui::Button("Quit", ImVec2(AppState::windowWidth / 3, 50)))
				{
					glfwSetWindowShouldClose(Input::window, true);
				}
				if (Input::IsEscapePressed())
				{
					glfwSetWindowShouldClose(Input::window, true);
				}
				ImGui::Unindent(AppState::windowWidth / 3);

				ImGui::End();
				break;
			}
			case StateMenu::OPTION:
			{
				ImGui::Begin("Options", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
				ImGui::SetWindowPos(ImVec2(0, -20));
				ImGui::SetWindowSize((ImVec2(AppState::windowWidth, AppState::windowHeight + 20)));

				ImGui::Indent(AppState::windowWidth / 2);
				ImGui::Dummy(ImVec2(0, 110));
				ImGui::Text("Options");
				ImGui::Unindent(AppState::windowWidth / 2);

				ImGui::Dummy(ImVec2(0, 60));
				SetButtonInOptionImgui("Forward", Input::Button::FORWARD);
				ImGui::Dummy(ImVec2(0, 40));
				SetButtonInOptionImgui("Backward", Input::Button::BACKWARD);
				ImGui::Dummy(ImVec2(0, 40));
				SetButtonInOptionImgui("Right", Input::Button::RIGHT);
				ImGui::Dummy(ImVec2(0, 40));
				SetButtonInOptionImgui("Left", Input::Button::LEFT);
				ImGui::Dummy(ImVec2(0, 40));
				SetButtonInOptionImgui("Jump", Input::Button::JUMP);


				if (Input::IsEscapePressed())
				{
					if (buttonSelected != Input::Button::NULLBUTTON)
						buttonSelected = Input::Button::NULLBUTTON;
					stateMenu = StateMenu::MAINMENU;
				}
				ImGui::End();
				break;
			}
			case StateMenu::EDITOR:
			{
				SwitchMode(AppState::Mode::EDITOR);
				AppState::sceneMode = AppState::Mode::EDITOR;
			}
			default:
				break;
			}
		}
	}

	void Scene::LoadResources()
	{
		ResourceManager::GetInstance().beginTime = std::chrono::high_resolution_clock::now();

		Shader* mainShader = ResourceManager::Create<Shader>("mainShader", "Resources/Shaders/");
		mainShader->SetShaders("Resources/Shaders/MainVertexShader.vert", "Resources/Shaders/MainFragmentShader.frag");

		Shader* noLightsShader = ResourceManager::Create<Shader>("noLightsShader", "Resources/Shaders/");
		noLightsShader->SetShaders("Resources/Shaders/NoLights.vert", "Resources/Shaders/NoLights.frag");

		Shader* ColliderShader = ResourceManager::Create<Shader>("colliderShader", "Resources/Shaders/");
		ColliderShader->SetShaders("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag");

		Shader* flatShader = ResourceManager::Create<Shader>("flatShader", "Resources/Shaders/");
		flatShader->SetShaders("Resources/Shaders/FlatShadingVertex.vert", "Resources/Shaders/FlatShadingFragment.frag");

		Shader* gouraudShader = ResourceManager::Create<Shader>("gouraudShader", "Resources/Shaders/");
		gouraudShader->SetShaders("Resources/Shaders/GouraudShadingVertex.vert", "Resources/Shaders/GouraudShadingFragment.frag");
		
		Shader* PhongShader = ResourceManager::Create<Shader>("PhongShader", "Resources/Shaders/");
		PhongShader->SetShaders("Resources/Shaders/PhongShadingVertex.vert", "Resources/Shaders/PhongShadingFragment.frag");
		
		Shader* blinnPhongShader = ResourceManager::Create<Shader>("blinnPhongShader", "Resources/Shaders/");
		blinnPhongShader->SetShaders("Resources/Shaders/BlinnPhongShadingVertex.vert", "Resources/Shaders/BlinnPhongShadingFragment.frag");

		Shader* skyBoxShader = ResourceManager::Create<Shader>("skyBoxShader", "Resources/Shaders/");
		skyBoxShader->SetShaders("Resources/Shaders/SkyBoxVertex.vert", "Resources/Shaders/SkyBoxFragment.frag");

		Shader* reflectionShader = ResourceManager::Create<Shader>("reflectionShader", "Resources/Shaders/");
		reflectionShader->SetShaders("Resources/Shaders/ReflectionVertex.vert", "Resources/Shaders/ReflectionFragment.frag");


		std::string name = "skyboxTest";
		ResourceManager::Create<Model>(name, "Resources/Obj/skybox.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/skybox1.png");


		name = "boo";
		ResourceManager::Create<Model>(name, "Resources/Obj/boo.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/boo.png");

		name = "cube";
		ResourceManager::Create<Model>(name, "Resources/Obj/cube.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/brick.png");


		name = "goomba";
		ResourceManager::Create<Model>(name, "Resources/Obj/goomba.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/goomba.PNG");

		name = "burger";
		ResourceManager::Create<Model>(name, "Resources/Obj/hamburger.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/hamburger.png");

		name = "chest";
		ResourceManager::Create<Model>(name, "Resources/Obj/Chest.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/Chest.png");

		name = "ball";
		ResourceManager::Create<Model>(name, "Resources/Obj/ball.obj");
		ResourceManager::Create<Texture>(name + "Texture", "Resources/Textures/ball.PNG");

		name = "myCube";
		ResourceManager::CustomCreate(name, Model::CustomModel::CUBE);

		name = "mySphere";
		ResourceManager::CustomCreate(name, Model::CustomModel::SPHERE);

		name = "myCapsule";
		ResourceManager::CustomCreate(name, Model::CustomModel::CAPSULE);

		name = "skyBox";
		CubeMap* skyBox = ResourceManager::Create<CubeMap>(name, "Resources/Textures/");
		skyBox->SetFaces({ "px.png", "nx.png"  ,"ny.png", "py.png","pz.png" ,"nz.png" });
		skyBox->LoadResource();
	}

	void Scene::SwitchMode(AppState::Mode mode)
	{
		AppState::sceneMode = mode;

		switch (mode)
		{
		case AppState::Mode::EDITOR: 
			Renderer::currentVP = &camera.VP;
			Renderer::currentProj = &camera.ProjectionMatrix;
			Renderer::currentView = &camera.ViewMatrix;
			break;

		case AppState::Mode::PLAY:
			glfwSetCursorPos(Input::window, AppState::windowWidth / 2.0f, AppState::windowHeight / 2.0f);
			if (gameCamera) 
			{
				Renderer::currentVP = &gameCamera->VP;
				Renderer::currentProj = &gameCamera->ProjectionMatrix;
				Renderer::currentView = &gameCamera->ViewMatrix;
			}
			else 
			{
				Renderer::currentVP = &camera.VP;
				Renderer::currentProj = &camera.ProjectionMatrix;
				Renderer::currentView = &camera.ViewMatrix;
			}
			break;

		default: break;
		}
	}

	//--------------------------------------------------------------------------------------------------------------------
	//				UPDATES
	//--------------------------------------------------------------------------------------------------------------------

	void Scene::UpdateInspector()
	{
		ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoMove);

		ImGui::SetWindowPos(ImVec2(AppState::windowWidth - ImGui::GetWindowSize().x, 0));
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x, AppState::windowHeight));

		if (selectedObject)
		{
			ImGui::Indent();
			ImGui::Checkbox("##active", &selectedObject->active);
			ImGui::Unindent();
			ImGui::SameLine();

			std::string key = strrchr((selectedObject->name).c_str(), '#');
			std::string name = selectedObject->name.substr(0, selectedObject->name.size() - key.size() - 1);
			char inputBuffer[128];
#pragma warning(suppress : 4996)
			std::strcpy(inputBuffer, name.c_str());
			if (ImGui::InputText("##Name", inputBuffer, 128))
			{
				selectedObject->name = inputBuffer;
				selectedObject->name += '#' + key;
			}

			ImGui::Dummy(ImVec2(5, 5));
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::DragFloat3("Position", &selectedObject->transform.position.x, 0.25, -100, 100);
				ImGui::DragFloat3("Rotation", &selectedObject->transform.rotation.x, 0.25, -360, 360);
				ImGui::DragFloat3("Scale", &selectedObject->transform.scale.x, 0.01, 0.01, 100);
			}
			unsigned int index = 0;
			for (MonoBehaviour* c : selectedObject->components)
			{
				ImGui::NewLine();
				c->UpdateInspector(index);
				index++;
			}
			ImGui::NewLine();

			if (ImGui::BeginPopupContextItem("AddComponent"))
			{
				if (ImGui::BeginMenu("Mesh"))
				{
					if (ImGui::MenuItem("Cube"))
						AddMesh(selectedObject, "cube");

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Directional"))
						AddDirLight(selectedObject);

					if (ImGui::MenuItem("Point"))
						AddPointLight(selectedObject);

					if (ImGui::MenuItem("Spot"))
						AddSpotLight(selectedObject);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Collider"))
				{
					if (ImGui::MenuItem("Box"))
					{
						AddRigidbody(selectedObject);
						AddCollider<BoxCollider>(selectedObject, *rockMaterial);

					}

					if (ImGui::MenuItem("Sphere"))
					{
						AddRigidbody(selectedObject);
						AddCollider<SphereCollider>(selectedObject, *rockMaterial);
					}

					if (ImGui::MenuItem("Capsule"))
					{
						AddRigidbody(selectedObject);
						AddCollider<CapsuleCollider>(selectedObject, *rockMaterial);
					}


					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}
		}
		ImGui::End();
	}

	void Scene::UpdateHierarchy()
	{
		ImGui::Begin("Scene Hierarchy", 0, ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x, AppState::windowHeight));

		if (ImGui::BeginPopupContextItem("NewGameObject"))
		{
			if (ImGui::Selectable("Empty GameObject"))
			{
				AddEmptyObject("GameObject");
			}
			if (ImGui::Selectable("Cube"))
			{
				AddModelObject("cube", "cubeTexture", "mainShader");
				
				AddCollider<BoxCollider>(&gameObjects.back(), *rockMaterial);
			}
			if (ImGui::Selectable("Sphere"))
			{
				AddModelObject("mySphere", "cubeTexture", "mainShader");
				
				AddCollider<SphereCollider>(&gameObjects.back(), *rockMaterial);
			}
			ImGui::EndPopup();
		}

		UpdateImGuiGraph(sceneGraph);

		ImGui::NewLine();
		if (ImGui::Button("Add GameObject"))
		{
			ImGui::OpenPopup("NewGameObject");
		}
		ImGui::End();
	}

	void Scene::UpdateImGuiGraph(std::list<Graph>& nodeList)
	{
		for (Graph& node : nodeList)
		{
			if (!node.object->active)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));

			bool opened = ImGui::TreeNodeEx(node.object->name.c_str(), GetTreeFlags(node));
			if (ImGui::BeginPopupContextItem("NewChild"))
			{
				if (ImGui::Selectable("Empty GameObject"))
				{
					AddEmptyObject("GameObject", &node);
				}
				if (ImGui::Selectable("Cube"))
				{
					AddModelObject("cube", "cubeTexture", "mainShader", &node);
				}
				if (ImGui::Selectable("Capsule"))
				{
					AddModelObject("myCapsule", "cubeTexture", "mainShader", &node);
				}
				if (ImGui::Selectable("Delete"))
				{
					DeleteGameObject(node);
					ImGui::EndPopup();
					ImGui::TreePop();
					return;
				}
				ImGui::EndPopup();
			}
			if (ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopup("NewChild");
			}
			if (ImGui::IsItemClicked())
			{
				selectedObject = node.object;
			}
			if (opened)
			{
				if (node.children.size() > 0)
				{
					UpdateImGuiGraph(node.children);
				}
				ImGui::TreePop();
			}

			if (!node.object->active)
				ImGui::PopStyleColor();
		}
	}


	void Scene::UpdateGameObjects(std::list<Graph>& nodeList)
	{

		for (Graph& node : nodeList)
		{
			if (!node.object->active)
				continue;

			node.object->UpdateMVP();

			for (MonoBehaviour* c : node.object->components)
			{
				c->Update();
			}

			if (node.children.size() > 0)
			{
				UpdateGameObjects(node.children);
			}

		}

		nodeList.front().object->UpdateMVP();
	}


	//--------------------------------------------------------------------------------------------------------------------
	//				ADD/DELETE GAMEOBJECTS
	//--------------------------------------------------------------------------------------------------------------------

	void Scene::AddEmptyObject(std::string name, Graph* parent, Transform transform)
	{
		std::string number = std::to_string(id);
		GameObject newObject(name + "##" + number); id++;

		gameObjects.push_back(newObject);

		if (parent)
		{
			Graph node(&gameObjects.back(), parent);
			parent->children.push_back(node);
			gameObjects.back().node = &parent->children.back();
		}

		else
		{
			Graph node(&gameObjects.back());
			sceneGraph.push_back(node);
			gameObjects.back().node = &sceneGraph.back();
		}

		gameObjects.back().SetModel(transform);
	}

	void Scene::AddEmptyObject(std::string name, Transform transform)
	{
		AddEmptyObject(name, WorldNode, transform);
	}

	void Scene::AddModelObject(std::string modelName, std::string textureName, std::string shaderName, Graph* parent, Transform transform, Type meshType)
	{
		std::string number = std::to_string(id);
		AddEmptyObject(modelName, parent, transform);
		AddMesh(&gameObjects.back(), modelName, meshType, textureName, shaderName);
	}

	void Scene::AddSkybox(std::string modelName, std::string textureName, std::string shaderName, Transform transform)
	{
		Renderer::skybox.push_back(AddMesh(modelName, textureName, shaderName));
	}

	void Scene::AddPointLightObject(Vector3 position, Vector3 color, Graph* parent)
	{
		if (Renderer::currentPointLights.size() >= MAX_POINT_LIGHTS)
		{
			DEBUG_LOG("Max lights number (%i) reached. New light not added.", MAX_POINT_LIGHTS);
			return;
		}

		AddEmptyObject("PointLight", parent, Transform(position, Vector3(), Vector3(1, 1, 1)));
		AddPointLight(&gameObjects.back(), color);
	}

	void Scene::AddDirLightObject(Vector3 rotation, Vector3 color, Graph* parent)
	{
		if (Renderer::currentDirectionalLights.size() >= MAX_DIR_LIGHTS)
		{
			DEBUG_LOG("Max directional lights number (1) reached. New light not added.");
			return;
		}

		AddEmptyObject("DirectionalLight", parent, Transform(Vector3(), rotation, Vector3(1, 1, 1)));
		AddDirLight(&gameObjects.back(), color);
	}

	void Scene::AddSpotLightObject(Vector3 position, Vector3 rotation, float angle, Vector3 color, Graph* parent)
	{
		if (Renderer::currentSpotLights.size() >= MAX_SPOT_LIGHTS)
		{
			DEBUG_LOG("Max spot lights number (1) reached. New light not added.");
			return;
		}

		AddEmptyObject("SpotLight", parent, Transform(position, rotation, Vector3(1, 1, 1)));
		AddSpotLight(&gameObjects.back(), angle, color);
	}

	void Scene::DeleteGameObject(Graph& node)
	{
		node.object->Delete();
		if (node.parent)
		{
			std::list<Graph>::iterator position = node.parent->children.begin();
			for (Graph& children : node.parent->children)
			{
				if (&children == &node)
				{
					node.parent->children.erase(position);
					break;
				}
				position++;
			}
		}
		else
		{
			std::list<Graph>::iterator position = sceneGraph.begin();
			for (Graph& children : sceneGraph)
			{
				if (&children == &node)
				{
					sceneGraph.erase(position);
					break;
				}
				position++;
			}
		}

		std::list<GameObject>::iterator position = gameObjects.begin();
		for (GameObject& children : gameObjects)
		{
			if (&children == node.object)
			{
				gameObjects.erase(position);
				break;
			}
			position++;
		}
	}

//--------------------------------------------------------------------------------------------------------------------
//				ADD COMPONENTS
//--------------------------------------------------------------------------------------------------------------------

void Scene::AddMesh(GameObject* object, std::string modelName, Type meshType, std::string textureName, std::string shaderName)
{
	Mesh* mesh = new Mesh;
	mesh->data = ResourceManager::Get<Model>(modelName);
	mesh->shader = ResourceManager::Get<Shader>(shaderName);
	mesh->texture = ResourceManager::Get<Texture>(textureName);
	mesh->meshType = meshType;
	//mesh->data->SetBuffersAndVAO();
	MonoBehaviour* component = (MonoBehaviour*)mesh;

	component->gameObject = object;
	object->components.push_back(component);
}

Mesh* Scene::AddMesh(std::string modelName, std::string textureName, std::string shaderName)
{
	Mesh* mesh = new Mesh;
	mesh->data = ResourceManager::Get<Model>(modelName);
	mesh->shader = ResourceManager::Get<Shader>(shaderName);
	mesh->texture = ResourceManager::Get<Texture>(textureName);
	return mesh;
}

void Scene::AddPointLight(GameObject* object, Vector3 color)
{
	if (Renderer::currentPointLights.size() >= MAX_POINT_LIGHTS)
	{
		DEBUG_LOG("Max lights number (%i) reached. New light not added.", MAX_POINT_LIGHTS);
		return;
	}

	PointLight* light = new PointLight(color);
	light->gameObject = object;
	MonoBehaviour* component = (MonoBehaviour*)light;

	object->components.push_back(component);
	Renderer::currentPointLights.push_back((PointLight*)component);
}

void Scene::AddDirLight(GameObject* object, Vector3 color)
{
	if (Renderer::currentDirectionalLights.size() >= MAX_DIR_LIGHTS)
	{
		DEBUG_LOG("Max lights number (%i) reached. New light not added.", MAX_DIR_LIGHTS);
		return;
	}

	DirectionalLight* light = new DirectionalLight(color);
	light->gameObject = object;
	MonoBehaviour* component = (MonoBehaviour*)light;

	object->components.push_back(component);
	Renderer::currentDirectionalLights.push_back((DirectionalLight*)component);
}

void Scene::AddSpotLight(GameObject* object, float angle, Vector3 color)
{
	if (Renderer::currentSpotLights.size() >= MAX_SPOT_LIGHTS)
	{
		DEBUG_LOG("Max lights number (%i) reached. New light not added.", MAX_SPOT_LIGHTS);
		return;
	}

	SpotLight* light = new SpotLight(color);
	light->gameObject = object;
	light->angle = angle;
	MonoBehaviour* component = (MonoBehaviour*)light;

	object->components.push_back(component);
	Renderer::currentSpotLights.push_back((SpotLight*)component);
}

void Scene::AddCamera(GameObject* object)
{
	//if (gameCamera != nullptr)
	//{
	//	DEBUG_LOG("Max camera number 1 reached. New Camera not added.");
	//	return;
	//}
	newcam = true;
	Camera* newCamera = new Camera();
	newCamera->gameObject = object;
	newCamera->Init(AppState::windowWidth, AppState::windowHeight);
	MonoBehaviour* component = (MonoBehaviour*)newCamera;

	object->components.push_back(component);
	gameCamera = (Camera*)component;
}

template <class T>
void Scene::AddCollider(GameObject* object, PxMaterial& material, Vector3 center, Vector3 size, bool trigger, bool isStatic)
{
	PxPhysics* physics = mPhysXManager.getPhysics();
	PxScene* scene = mPhysXManager.getScene();
	Matrix4 worldModel = object->GetWorldModel();
	if (worldModel.matrix[0][2] == 0 || worldModel.matrix[1][1] == 0 || worldModel.matrix[2][0] == 0){
	printf("Zero\n");
	}
		if (typeid(T) == typeid(BoxCollider))
		{
			T* collider = new T();
			BoxCollider* box = (BoxCollider*)collider;
			box->gameObject = object;
			box->center = center;
			box->size = size;
			box->isTrigger = trigger;
			box->geometry = PxBoxGeometry(abs(worldModel.matrix[0][0]) * size.x*0.5f, abs(worldModel.matrix[1][1]) * size.y * 0.5f, abs(worldModel.matrix[2][2]) * size.z * 0.5f);
			box->Init(*physics,material,*scene);
			MonoBehaviour* component = (MonoBehaviour*)box;
			object->components.push_back(component);
			/*Collision::currentColliders.push_back((Collider*)component);*/
		}
		else if (typeid(T) == typeid(SphereCollider))
		{
			T* collider = new T();
			SphereCollider* sphere = (SphereCollider*)collider;
			sphere->gameObject = object;
			sphere->center = center;
			sphere->radius = size.x;
			sphere->geometry = PxSphereGeometry(abs(worldModel.matrix[0][0]) * size.x);
			sphere->isTrigger = trigger;
			sphere->Init(*physics, material,*scene);
			MonoBehaviour* component = (MonoBehaviour*)sphere;
			object->components.push_back(component);
			/*Collision::currentColliders.push_back((Collider*)component);*/
		}
		else if (typeid(T) == typeid(CapsuleCollider))
		{
			T* collider = new T();
			CapsuleCollider* capsule = (CapsuleCollider*)collider;
			capsule->gameObject = object;
			//capsule->InitCapsule();
			capsule->center = center;
			capsule->radius = size.x;
			capsule->height = size.y;
			capsule->isTrigger = trigger;
			capsule->geometry = PxCapsuleGeometry(abs(worldModel.matrix[0][0]) * size.x, abs(worldModel.matrix[1][1]) * size.y * 0.5f);
			capsule->Init(*physics, material,*scene);
			MonoBehaviour* component = (MonoBehaviour*)capsule;
			object->components.push_back(component);
			/*Collision::currentColliders.push_back((Collider*)component);*/
		}
	}

	template<class T>
	void Scene::AddScript(GameObject* object)
	{
		bool IsInherit = std::is_base_of<Script, T>::value;
		Assert(!IsInherit);

		T* script = new T();
		Script* newScript = (Script*)script;
		newScript->gameObject = object;
		newScript->Init();
		MonoBehaviour* component = (MonoBehaviour*)newScript;

		object->components.push_back(component);
	}

	void Scene::AddRigidbody(GameObject* object)
	{
		Rigidbody* rb = new Rigidbody();
		rb->gameObject = object;
		rb->Init();
		MonoBehaviour* component = (MonoBehaviour*)rb;

		object->components.push_back(component);
	}

	ImGuiTreeNodeFlags Scene::GetTreeFlags(Graph& node)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
		if (node.children.size() <= 0)
			treeNodeFlags += ImGuiTreeNodeFlags_Leaf;
		if (selectedObject == node.object)
			treeNodeFlags += ImGuiTreeNodeFlags_Selected;

		return treeNodeFlags;
	}

	void Scene::SetButtonInOptionImgui(std::string name, Input::Button moveChange)
	{
		uint32_t f = Input::GetKey(moveChange);
		std::string control;
		switch (f)
		{
		case 32:
			control = "SPACE";
			break;
		default:
			control = (char)f;
			break;
		}
		ImGui::Indent(AppState::windowWidth * 2 / 5);
		ImGui::Text(name.c_str());
		ImGui::SameLine(200);
		ImGui::Unindent(AppState::windowWidth * 2 / 5);
		ImGui::Indent(AppState::windowWidth / 2);

		if (ImGui::Button(control.c_str(), ImVec2(AppState::windowHeight / 4, 50.0f)))
		{
			buttonSelected = moveChange;
		}
		if (moveChange == buttonSelected)
		{
			ImGui::SameLine(AppState::windowWidth * 4 / 6);
			ImGui::Text("button selected");
		}
		ImGui::Unindent(AppState::windowWidth / 2);

		if (buttonSelected != Input::Button::NULLBUTTON)
		{
			for (int i = 0; i < GLFW_KEY_LAST; i++)
			{
				if (glfwGetKey(Input::window, i) == GLFW_PRESS)
				{
					if (i == GLFW_KEY_ESCAPE)
					{
						continue;
					}
					if (Input::CheckInput(i))
					{
						if (moveChange == buttonSelected)
						{
							ImGui::SameLine(AppState::windowWidth * 4 / 5);
							ImGui::Text("Already used !!");
							break;
						}
					}
					else
					{
						Input::ChangeControl(buttonSelected, i);
						buttonSelected = Input::Button::NULLBUTTON;
						break;
					}
				}
			}
		}
	}
}