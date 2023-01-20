#include "mouse.h"

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };


void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action != GLFW_RELEASE) {
		if (!buttons[button]) {
			buttons[button] = true;
		}
	}
	else {
		buttons[button] = false;
	}
	buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::mouseWheelCallback(GLFWwindow* window) {

}

bool Mouse::button(int button) {
	return buttons[button];
}

bool Mouse::buttonChanged(int button) {
	bool ret = buttonsChanged[button];
	buttonsChanged[button] = false;
	return ret;
}

bool Mouse::buttonWentUp(int button) {
	return !buttons[button] && buttonChanged(button);
}

bool Mouse::buttonWentDown(int button) {
	return buttons[button] && buttonChanged(button);
}