#pragma once

#include "nate_camera.hpp"
#include "nate_game_object.hpp"

#include <vulkan/vulkan.h>

#define MAX_LIGHTS 10

struct PointLight {
	glm::vec4 position{};
	glm::vec4 color{};
};

namespace nate {

	struct GlobalUbo {
		glm::mat4 projection{ 1.0f };
		glm::mat4 view{ 1.0f };
		glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		NateCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		NateGameObject::Map& gameObjects;
	};
}