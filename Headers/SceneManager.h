#pragma once
#include <vector>
#include "Scene.h"



namespace Resources
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		Scene* currentScene;
		std::vector<Scene*> allScene;
		void Update();
		void Init();
	private:
		void AddNewScene();
	};
}
