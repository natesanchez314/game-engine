#include "first_app.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>

namespace nate
{
	FirstApp::FirstApp()
	{
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{ }

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ nateDevice, nateRenderer.getSwapChainRenderPass() };

		while (!nateWindow.shouldClose())
		{
			glfwPollEvents();
			if (auto commandBuffer = nateRenderer.beginFrame())
			{
				nateRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				nateRenderer.endSwapChainRenderPass(commandBuffer);
				nateRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(nateDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<NateModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		auto nateModel = std::make_shared<NateModel>(nateDevice, vertices);

		auto triangle = NateGameObject::createGameObject();
		triangle.model = nateModel;
		triangle.color = { 0.1f, 0.8f, 0.1f };
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.scale = { 2.0f, 0.5f };
		triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}