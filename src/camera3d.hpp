#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace nate {

	class Camera3d {
	private:
		glm::mat4 projMat{ 1.0f };
		glm::mat4 viewMat{ 1.0f };
		glm::mat4 invViewMat{ 1.0f };

	public:
		void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);
		const glm::mat4 getProjection() const { return projMat; }
		const glm::mat4 getView() const { return viewMat; }
		const glm::mat4 getInvView() const { return invViewMat; }
	};
}