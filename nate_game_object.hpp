#pragma once

#include "nate_model.hpp"

#include <memory>

namespace nate
{
	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMat{ {c, s}, {-s, c} };
			glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} };
			return rotMat * scaleMat; 
		}
	};

	class NateGameObject
	{
	public:
		using id_t = unsigned int;
		std::shared_ptr<NateModel> model{};
		glm::vec3 color{};
		Transform2dComponent transform2d{};

	private:
		id_t id;

	public:
		static NateGameObject createGameObject()
		{
			static id_t currentId = 0;
			return NateGameObject(currentId++);
		}

		id_t getId() { return id; }

		NateGameObject(const NateGameObject&) = delete;
		NateGameObject& operator=(const NateGameObject&) = delete;
		NateGameObject(NateGameObject&&) = default;
		NateGameObject& operator=(NateGameObject&&) = default;

	private:
		NateGameObject(id_t objId)
			:id(objId)
		{ }
	};
}