#include "first_app.hpp"

#include "simple_render_system.hpp"
#include "nate_camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "nate_buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <cassert>
#include <chrono>
#include <numeric>

namespace nate {

    struct GlobalUbo {
        glm::mat4 projectionView{ 1.0f };
        glm::vec3 lightDirection = glm::vec3{ 1.0f, -3.0f, -1.0f };
    };

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() { }

	void FirstApp::run() {
        auto minOffsetAlignment = std::lcm(
            nateDevice.properties.limits.minUniformBufferOffsetAlignment,
            nateDevice.properties.limits.nonCoherentAtomSize
        );

        NateBuffer globalUboBuffer{
            nateDevice,
            sizeof(GlobalUbo),
            NateSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            minOffsetAlignment
        };
        globalUboBuffer.map();

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
                int frameIndex = nateRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera
                };
                // Update phase
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                globalUboBuffer.writeToIndex(&ubo, frameIndex);
                globalUboBuffer.flushIndex(frameIndex);

                // Render phase
				nateRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				nateRenderer.endSwapChainRenderPass(commandBuffer);
				nateRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(nateDevice.device());
	}

	void FirstApp::loadGameObjects() {
        //std::shared_ptr<NateModel> nateModel = NateModel::createModelFromFile(nateDevice, "res/colored_cube.obj");
		std::shared_ptr<NateModel> nateModel0 = NateModel::createModelFromFile(nateDevice, "res/smooth_vase.obj");
		std::shared_ptr<NateModel> nateModel1 = NateModel::createModelFromFile(nateDevice, "res/flat_vase.obj");

        auto gameObject0 = NateGameObject::createGameObject();
        gameObject0.model = nateModel0;
        gameObject0.transform.translation = { -0.5f, 0.5f, 2.5f };
        gameObject0.transform.scale = glm::vec3(3.0f);

        gameObjects.push_back(std::move(gameObject0));

        auto gameObject1 = NateGameObject::createGameObject();
        gameObject1.model = nateModel1;
        gameObject1.transform.translation = { 0.5f, 0.5f, 2.5f };
        gameObject1.transform.scale = glm::vec3(3.0f);

        gameObjects.push_back(std::move(gameObject1));
	}
}