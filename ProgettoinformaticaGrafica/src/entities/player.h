#pragma once

#include "entitybase.h"
#include "../io/mouse.h"
#include "../io/camera.h"
#include "../physics/rigidbody.h"

#define DEBUG 0

class Player // : public EntityBase
{
public:
	Player(const std::string& name, const std::string& modelId, Scene* scene);

	glm::vec3 playerPos;
	void setPlayerCamera(Camera* camera);

	virtual void init(glm::vec3 size, float mass, glm::vec3 pos);
	virtual void update(double dt);

	//virtual void onCollide(EntityBase* other) = 0;

//protected:

	const std::string name;
	const std::string modelId;
	Scene* scene;

	RigidBody* rigidBody;
	Camera* camera;
    
	void collectGlass();
	void fillGlass();
	void throwGlass();
	

private:
	Glass* glassInHand;
	bool hasGlass;

};

