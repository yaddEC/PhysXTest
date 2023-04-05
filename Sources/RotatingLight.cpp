#include "RotatingLight.h"
#include "AppState.h"

using namespace Core::Maths;

namespace Game
{
	void RotatingLight::UpdateScript()
	{
		angle += AppState::deltatime * speed;
		float x = pivot->x + radius * cos(angle);
		float y = pivot->y;
		float z = pivot->z + radius * sin(angle);
		gameObject->transform.position = Vector3(x, y, z);
	}

	bool RotatingLight::UpdateComponent(std::string* id)
	{
		bool opened = ImGui::CollapsingHeader(("Script : RotatingLight##" + *id).c_str(), ImGuiTreeNodeFlags_DefaultOpen);

		bool popUp = false;
		if (ImGui::IsItemClicked(1))
		{
			popUp = true;
		}

		if (opened)
		{
			ImGui::Checkbox(("active##" + *id).c_str(), &active);
		}

		return popUp;
	}

	void RotatingLight::SetPivot(Core::Maths::Vector3* pos)
	{
		pivot = pos;
	}

	void RotatingLight::SetRadius(float rad)
	{
		radius = rad;
	}

	void RotatingLight::SetSpeed(float spe)
	{
		speed = spe;
	}

	void RotatingLight::SetAll(Core::Maths::Vector3* pos, float rad, float spe)
	{
		pivot = pos;
		radius = rad;
		speed = spe;
	}
}