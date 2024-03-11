#pragma once

#include "nate_camera.hpp"

#include <vulkan/vulkan.h>

namespace nate {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		NateCamera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
}