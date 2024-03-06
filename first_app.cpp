#include "first_app.hpp"

#include "simple_render_system.hpp"
#include "nate_camera.hpp"
#include "keyboard_movement_controller.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>
#include <chrono>

namespace nate {

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() { }

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ nateDevice, nateRenderer.getSwapChainRenderPass() };
        NateCamera camera{};
        camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

        auto viewerObject = NateGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!nateWindow.shouldClose()) {
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(nateWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = nateRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.01f, 10.0f);

			if (auto commandBuffer = nateRenderer.beginFrame()) {
				nateRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				nateRenderer.endSwapChainRenderPass(commandBuffer);
				nateRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(nateDevice.device());
	}

	void FirstApp::loadGameObjects() {
        //std::shared_ptr<NateModel> nateModel = NateModel::createModelFromFile(nateDevice, "res/colored_cube.obj");
		std::shared_ptr<NateModel> nateModel = NateModel::createModelFromFile(nateDevice, "res/smooth_vase.obj");

        auto gameObject = NateGameObject::createGameObject();
        gameObject.model = nateModel;
        gameObject.transform.translation = { 0.0f, 0.0f, 2.5f };
        gameObject.transform.scale = { 0.5f, 0.5f, 0.5f };

        gameObjects.push_back(std::move(gameObject));
	}
}