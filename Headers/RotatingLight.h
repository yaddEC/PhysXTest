#pragma once
#include "Script.h"
#include "MyMaths.h"

namespace Game
{
	class RotatingLight : public Script
	{
	public:
		void Init() override {};
		void UpdateScript() override;
		bool UpdateComponent(std::string* id) override;

		void SetPivot(Core::Maths::Vector3* pos);
		void SetRadius(float rad);
		void SetSpeed(float spe);
		void SetAll(Core::Maths::Vector3* pos, float rad, float spe);
	private:
		Core::Maths::Vector3* pivot;
		float radius;
		float speed;
		float angle = 0;
	};
}