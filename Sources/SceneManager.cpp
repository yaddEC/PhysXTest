#include "SceneManager.h"

using namespace Resources;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
	for (int i = 0; i < allScene.size(); i++)
	{
		free(allScene[i]);
	}
}

void SceneManager::Init()
{
	for (int i = 0; i < 3; i++)
	{
		Scene* newScene = new Scene;
		allScene.push_back(newScene);
	}
	allScene[0]->name = "Platformer Skybox";
	allScene[1]->name = "Basic lighting";
}


void SceneManager::Update()
{
	if (!currentScene)
	{
		Input::ShowCursor();
		AppState::sceneMode = AppState::Mode::PLAY;
		ImGui::Begin("Choose Scene", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(0, -20));
		ImGui::SetWindowSize((ImVec2(AppState::windowWidth, AppState::windowHeight + 20)));
		ImGui::Text("\n\n\n\n\n\n\n\n");
		ImGui::Indent((AppState::windowWidth / 2) - 45);
		ImGui::Text("Selector Scene");
		ImGui::Unindent((AppState::windowWidth / 2) - 45);
		//ImGui::Indent(AppState::windowWidth / 3);

		for (int i = 0; i < allScene.size(); i++)
		{
			ImGui::Text("\n\n\n\n");
			std::string name;
			if (allScene[i]->name == "")
			{
				name = "Scene " + std::to_string(i);
			}
			else
			{
				name = allScene[i]->name;
			}
			ImGui::Indent((AppState::windowWidth / 3));
			if (ImGui::Button(name.c_str(), ImVec2(AppState::windowWidth / 3, 50)))
			{
				allScene[i]->Init(i);
				currentScene = allScene[i];

			}
			ImGui::Unindent((AppState::windowWidth / 3));

		}
		ImGui::Text("\n\n\n\n");
		ImGui::Indent((AppState::windowWidth / 3));
		if (ImGui::Button("Quit", ImVec2(AppState::windowWidth / 3, 50)))
		{
			glfwSetWindowShouldClose(Input::window, true);
		}
		ImGui::Unindent((AppState::windowWidth / 3));
		if (Input::IsEscapePressed())
		{
			glfwSetWindowShouldClose(Input::window, true);
		}
		//ImGui::Unindent(AppState::windowWidth / 3);

		ImGui::End();
	}
	else
	{
		currentScene->Update(*this);
	}
}


void SceneManager::AddNewScene()
{

}
