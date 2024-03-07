#pragma once

#include "nate_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace nate {
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	class NateGameObject {
	public:
		using id_t = unsigned int;
		std::shared_ptr<NateModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		id_t id;

	public:
		static NateGameObject createGameObject() {
			static id_t currentId = 0;
			return NateGameObject(currentId++);
		}

		id_t getId() { return id; }

		NateGameObject(const NateGameObject&) = delete;
		NateGameObject& operator=(const NateGameObject&) = delete;
		NateGameObject(NateGameObject&&) = default;
		NateGameObject& operator=(NateGameObject&&) = default;

	private:
		NateGameObject(id_t objId) : id(objId) { }
	};
}