#include "glass.h"

Glass::Glass(const std::string & name, const std::string & modelId, Scene * scene)
	: name(name), modelId(modelId), scene(scene), isEmpty(true)
{

}

void Glass::init(glm::vec3 size, float mass, glm::vec3 pos)
{
		rigidBody = scene->generateInstance(modelId, size, mass, pos);
		std::cerr << "Create glass"\n";
}



void Glass::update(double dt)
{
	if (scene)
	{

		// set camera direction
		double mouseDX = Mouse::getDX();
		double mouseDY = Mouse::getDY();
		if (mouseDX != 0.0 || mouseDY != 0)
		{
			camera->updateCameraDirection(mouseDX, mouseDY);
		}

		// set camera zoom
		//cameras[activeCamera]->updateCameraZoom(Mouse::getScrollDY());

#if DEBUG
		if (Keyboard::key(GLFW_KEY_SPACE))
		{
			camera->updateCameraPos(CameraDirection::UP, dt);
		}
		if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
		{
			camera->updateCameraPos(CameraDirection::DOWN, dt);
		}
#endif


		float rt = joystick.axesState(GLFW_JOYSTICK_AXES_LEFT_TRIGGER);

		if (rt > 0.5f)
		{
			if (!axeThrown)
			{
				throwAxe();
				axeThrown = true;
			}
		}
		else
		{
			axeThrown = false;
		}
	}
	else
	{
		axeThrown = false;
	}




}
}