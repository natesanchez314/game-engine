#include "first_app.hpp"

#include "simple_render_system.hpp"
#include "point_light_system.hpp"
#include "camera3d.hpp"
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

namespace nate {

	FirstApp::FirstApp() {
        globalPool = NateDescriptorPool::Builder(nateDevice)
            .setMaxSets(NateSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, NateSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
		loadGameObjects();
	}

	FirstApp::~FirstApp() { }

	void FirstApp::run() {

        std::vector<std::unique_ptr<NateBuffer>> uboBuffers(NateSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<NateBuffer>(
                nateDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
            uboBuffers[i]->map();
        }

        auto globalSetLayout = NateDescriptorSetLayout::Builder(nateDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(NateSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            NateDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

		SimpleRenderSystem simpleRenderSystem{ 
            nateDevice, 
            nateRenderer.getSwapChainRenderPass(), 
            globalSetLayout->getDescriptorSetLayout()
        };
        PointLightSystem pointLightSystem{
            nateDevice,
            nateRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()
        };
        Camera3d camera{};
        camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

        auto viewerObject = NateGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
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
            camera.setPersProj(glm::radians(50.0f), aspect, 0.01f, 100.0f);

			if (auto commandBuffer = nateRenderer.beginFrame()) {
                int frameIndex = nateRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects
                };
                // Update phase
                GlobalUbo ubo{};
                ubo.proj = camera.getProj();
                ubo.view = camera.getView();
                ubo.invView = camera.getInvView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // Render phase
				nateRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
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
        std::shared_ptr<NateModel> quad = NateModel::createModelFromFile(nateDevice, "res/quad.obj");

        auto gameObject0 = NateGameObject::createGameObject();
        gameObject0.model = nateModel0;
        gameObject0.transform.translation = { -0.5f, 0.5f, 0.0f };
        gameObject0.transform.scale = glm::vec3(3.0f);

        gameObjects.emplace(gameObject0.getId(), std::move(gameObject0));

        auto gameObject1 = NateGameObject::createGameObject();
        gameObject1.model = nateModel1;
        gameObject1.transform.translation = { 0.5f, 0.5f, 0.0f };
        gameObject1.transform.scale = glm::vec3(3.0f);

        gameObjects.emplace(gameObject1.getId(),std::move(gameObject1));

        auto floor = NateGameObject::createGameObject();
        floor.model = quad;
        floor.transform.translation = { 0.0f, 0.5f, 0.0f };
        floor.transform.scale = glm::vec3(3.0f);

        gameObjects.emplace(floor.getId(), std::move(floor));

        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto pointLight = NateGameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.0f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                { 0.0f, -1.0f, 0.0f }
            );
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
	}
}