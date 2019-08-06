#pragma once
#include "Common/Common.h"

#include <stack>
#include <memory>

#include <GLFW/glfw3.h>

#include "StateBase.h"
#include "System/Window/Window.h"

namespace Oath {

	namespace APP {

		class Application {
		public:
			Application(const String& Name, float width, float height);
			~Application();

			template<typename T, typename... Args>
			void runLoop(Args&&... args);

			template<typename T, typename... Args>
			void pushState(Args&&... args);

			inline void popState(){ PopState = true; }
			inline void popAllState(){ PopAllStates = true; }

			inline APP::StateBase& getState() { return *states.top(); }
			inline GLFWwindow* getWindow() { return window.window; }

			Event::Dispatcher dispatcher;
		private:
			void loop();

			WINDOW::Window window;

			std::stack<std::unique_ptr<StateBase>> states;
			bool PopState = false, PopAllStates = false;
		};

		template<typename T, typename ...Args>
		inline void Application::runLoop(Args && ...args)
		{
			pushState<T>(std::forward<Args>(args)...);
			loop();
		}

		template<typename T, typename ...Args>
		inline void Application::pushState(Args && ...args)
		{
			if (states.size() != 0)
				states.top()->onLeave();
			states.emplace(std::make_unique<T>(this, std::forward<Args>(args)...));
			states.top()->onOpen();
		}
	}
}