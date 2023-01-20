#include "player.h"

Player::Player(const std::string& name, const std::string& modelId, Scene* scene)
	: name(name), modelId(modelId), scene(scene), camera(nullptr)
{

}

void Player::setPlayerCamera(Camera* camera)
{
	this->camera = camera;
	//cameraBR = new BoundingRegion(camera->cameraPos, 1.0f);
}

void Player::init(glm::vec3 size, float mass, glm::vec3 pos)
{
	rigidBody = scene->generateInstance(modelId, size, mass, pos);
}

void Player::update(double dt)
{
	if (scene)
	{
		//UPDATE CAMMINATA


		if (Keyboard::key(GLFW_KEY_UP))
		{
		//	rigidBody->pos =
		}
		if (Keyboard::key(GLFW_KEY_DOWN))
		{
		//	camera->updateCameraPos(CameraDirection::DOWN, dt);

		}
	}


		
	
}

void Player::collectGlass() {

	if (scene)
	{

	}

}

void Player::fillGlass() {

	if (scene)
	{

	}

}

void Player::throwGlass() {
	if (scene)
	{
		RigidBody* rb = scene->generateInstance("glass", glm::vec3(.50f), 1.0f, rigidBody -> pos, glm::vec3(0.0, 0.0, 0.0));
		//scene->removeAxe();
		if (rb) {
			// instance generated
		//	rb->transferEnergy(100.0f, camera->cameraFront);
			//rb->applyAcceleration(Environment::gravitationalAcceleration);
		}
		

	}
}