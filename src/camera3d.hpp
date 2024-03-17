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
		void setViewDir(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

		void setOrthoProj(float left, float right, float top, float bottom, float near, float far);
		void setPersProj(float fovy, float aspect, float near, float far);
		const glm::mat4& getProj() const { return projMat; }
		const glm::mat4& getView() const { return viewMat; }
		const glm::mat4& getInvView() const { return invViewMat; }
		const glm::vec3 getPos() const { return glm::vec3(invViewMat[3]); }
	};
}