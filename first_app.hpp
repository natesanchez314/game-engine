#pragma once

#include "nate_window.hpp"
#include "nate_device.hpp"
#include "nate_game_object.hpp"
#include "nate_renderer.hpp"

#include <memory>
#include <vector>

namespace nate
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		NateWindow nateWindow{ WIDTH, HEIGHT, "Vulkan is hard" };
		NateDevice nateDevice{ nateWindow };
		NateRenderer nateRenderer{ nateWindow, nateDevice };
		std::vector<NateGameObject> gameObjects;

	public:
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();
	};
}