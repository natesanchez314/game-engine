#pragma once

#include "nate_window.hpp"
#include "nate_pipeline.hpp"

namespace nate
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		NateWindow nateWindow{WIDTH, HEIGHT, "Vulkan is hard"};
		NatePipeline natePipeline{"simple_shader.vert.spv", "simple_shader.frag.spv"};

	public:
		void run();
	};
}