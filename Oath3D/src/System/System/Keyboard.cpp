#include "Common/Common.h"
#include "Keyboard.h"

#include "GLFW/glfw3.h"

#include "System/Application/Application.h"

namespace Oath {

	bool Key::isKeyPressed(KeyboardKey key)
	{
		return glfwGetKey(APP::GApp->getWindow(), key) == GLFW_PRESS;
	}

	bool Key::isKeyHeld(KeyboardKey key)
	{
		int res = glfwGetKey(APP::GApp->getWindow(), key);
		return res == GLFW_PRESS || res == GLFW_REPEAT;
	}

	bool Key::isKeyReleased(KeyboardKey key)
	{
		return glfwGetKey(APP::GApp->getWindow(), key) == GLFW_RELEASE;
	}

	bool Key::isButtonReleased(MouseButton key)
	{
		return glfwGetMouseButton(APP::GApp->getWindow(), key) == GLFW_RELEASE;
	}

	bool Key::isButtonPressed(MouseButton key)
	{
		int res = glfwGetMouseButton(APP::GApp->getWindow(), key);
		return res == GLFW_PRESS || res == GLFW_REPEAT;
	}
}