#pragma once

#include "nate_window.hpp"
#include "nate_pipeline.hpp"
#include "nate_device.hpp"

namespace nate
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		NateWindow nateWindow{WIDTH, HEIGHT, "Vulkan is hard"};
		NateDevice nateDevice{nateWindow};
		NatePipeline natePipeline{nateDevice, "simple_shader.vert.spv", "simple_shader.frag.spv", NatePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

	public:
		void run();
	};
}