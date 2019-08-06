#include "Common/Common.h"
#include "Application.h"

#include "Component/Camera/Camera.h"
#include "System/Renderer/MasterRenderer.h"
#include "System/Renderer/Shader/Shader.h"

#include "ImGui/Include.h"

#include "System/System/FileSystem.h"

namespace Oath {
	namespace APP {

		Application* GApp = nullptr;

		bool HasFocus = true;

		void sizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, (GLsizei)width, (GLsizei)height);

			Settings::WIDTH = (float)width;
			Settings::HEIGHT = (float)height;

			if (Comp::GCam != nullptr)
				Comp::GCam->proj = perspective(Settings::FOV, (float)width / (float)height, Settings::ZNEAR, Settings::ZFAR);
			
			if (Render::GRenderer != nullptr)
				Render::GRenderer->resize(width, height);
		}

		void focusCallback(GLFWwindow* window, int focused)
		{
			if (focused)
				HasFocus = true;
			else if (!Settings::CONTINUE_WITH_LOST_FOCUS)
				HasFocus = false;
		}

		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{ }

		void dropCallback(GLFWwindow* window, int count, const char** paths)
		{ }

		Application::Application(const String& Name, float width, float height) :
			window(Name, width, height)
		{
			GApp = this;

			glfwSetWindowSizeCallback(window.window, sizeCallback);
			glfwSetWindowFocusCallback(window.window, focusCallback);
			glfwSetScrollCallback(window.window, scrollCallback);
			glfwSetDropCallback(window.window, dropCallback);

			File::DeleteAFile(Settings::LOG_FILE);
		}

		Application::~Application() {
			GApp = nullptr;
		}

		void Application::loop() {
			System::Clock Timer, RenderTimer;

			float fpsTimeCounter = 0.0;
			float updateTimer = 1.0;
			int FPS = 0, lastFPS = 0;

			while (!glfwWindowShouldClose(window.window) && !states.empty()) {

				if (!HasFocus) {
					glfwPollEvents();
					Sleep(100);
					continue;
				}

				float deltaTime = std::min((float)Timer.resetS(), 0.15f);
				fpsTimeCounter += deltaTime;
				updateTimer += deltaTime;
				
				auto& state = *states.top();
				
				bool shouldRender = true;
				while (updateTimer >= Settings::FRAME_TIME) {

					glfwPollEvents();
					
					state.world->processInteractions(Settings::FRAME_TIME);
					state.ecs.updateSystems(state.updatelist, Settings::FRAME_TIME);
					state.update(Settings::FRAME_TIME);

					updateTimer -= Settings::FRAME_TIME;
					shouldRender = true;

					if (PopAllStates) {

						PopAllStates = false;

						for (int i = 0; i <= states.size(); i++) {
							states.top()->onClose();
							states.pop();
						}

						if (!states.empty()) {
							state = *states.top();
							Render::GRenderer = &state.renderer;
							state.onResume();
						}

						break;
					}
					else if (PopState) {

						PopState = false;
						state.onClose();
						states.pop();

						if (!states.empty()) {
							state = *states.top();
							Render::GRenderer = &state.renderer;
							state.onResume();
						}

						break;
					}
				}

				if (shouldRender && !states.empty()) {

					if (Settings::USING_IMGUI) {
						ImGui_ImplOpenGL3_NewFrame();
						ImGui_ImplGlfw_NewFrame();
						ImGui::NewFrame();

						ImGui::Begin("FPS");
						ImGui::Text("FPS : %.2f ms ( %i FPS )", 1000.0 / (double)lastFPS, lastFPS);
						ImGui::End();
					}

					if (fpsTimeCounter >= 1.0) {
						lastFPS = FPS;
						fpsTimeCounter = 0;
						FPS = 0;

						ONLY_DEBUG(Render::Shader::ReloadShaders());
					}
					FPS++;

					float RenderTime = std::min((float)RenderTimer.resetS(), 0.15f);

					if (Settings::DEBUG_RENDER_BULLET)
						state.world->debugDraw();
					
					state.ecs.updateSystems(state.rendering, RenderTime);
					state.render(RenderTime);
					state.renderer.render();

					if (Settings::USING_IMGUI) {
						ImGui::Render();
						ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					}

					glfwSwapBuffers(window.window);
				}
			}
		}
		
	}
}