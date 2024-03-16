#pragma once

#include "nate_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

namespace nate {
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	struct PointLightComponent {
		float lightIntensity = 1.0f;
	};

	class NateGameObject {
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, NateGameObject>;

		glm::vec3 color{};
		TransformComponent transform{};

		std::shared_ptr<NateModel> model{};
		std::unique_ptr<PointLightComponent> pointLight = nullptr;

	private:
		id_t id;

	public:
		static NateGameObject createGameObject() {
			static id_t currentId = 0;
			return NateGameObject(currentId++);
		}

		static NateGameObject makePointLight(float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.0f));

		id_t getId() { return id; }

		NateGameObject(const NateGameObject&) = delete;
		NateGameObject& operator=(const NateGameObject&) = delete;
		NateGameObject(NateGameObject&&) = default;
		NateGameObject& operator=(NateGameObject&&) = default;

	private:
		NateGameObject(id_t objId) : id(objId) { }
	};
}