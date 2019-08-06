#include "Common/Common.h"
#include "Window.h"

#include "ImGui/Include.h"


namespace Oath {

	namespace WINDOW {

		void errorCallback(int error, const char* description)
		{
			LOGERROR("GLFW ERROR :", error, description);
		}

		void InitGL()
		{
			glewExperimental = GL_TRUE;
			GLCall(glewInit());

			GLCall(glCullFace(GL_BACK));
			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glEnable(GL_CULL_FACE));
			GLCall(glClearColor(0, 0, 0, 0));
		}

		Window::Window(const String & Name, float width, float height) {
			
			static bool HasInitGL = false;

			glfwSetErrorCallback(errorCallback);
			if (!HasInitGL)
				if (!glfwInit()) {
					LOGERROR("FAILED TO INITIALIZE GLFW");
					return;
				}

			window = glfwCreateWindow((int)width, (int)height, Name.c_str(), NULL, NULL);
			
			if (!window) {
				LOGERROR("FAILED TO CREATE WINDOW");
				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(window);
			glViewport(0, 0, (int)width, (int)height);

			if (!HasInitGL) {
				HasInitGL = true;
				InitGL();
			}

			device = alcOpenDevice(NULL);
			if (!device)
				LOGERROR("Failed to initialize OpenAL");
			context = alcCreateContext(device, NULL);
			if (!context)
				LOGERROR("Failed to initialize OpenAL");
			if (!alcMakeContextCurrent(context))
				LOGERROR("Failed to initialize OpenAL");

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			(void)ImGui::GetIO();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 130");

			ImGui::StyleColorsDark();
		}

		Window::~Window()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);
			alcCloseDevice(device);

			glfwDestroyWindow(window);
			glfwTerminate();
		}

		int Window::getWidth()
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			return width;
		}

		int Window::getHeight()
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			return height;
		}
	}
}