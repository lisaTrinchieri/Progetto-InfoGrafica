#pragma once

#include "entitybase.h"
#include "../physics/environment.h"
#include "../physics/rigidbody.h"
#include "../algorithms/states.hpp"
#include "../algorithms/trie.hpp"

class Client //: public EntityBase
{
public:

	Client(const std::string& name, const std::string& modelId, Scene* scene);

	virtual void init(glm::vec3 size, float mass, glm::vec3 pos);
	virtual void update(double dt);

	const std::string name;
	const std::string modelId;
	Scene* scene;

	RigidBody* rigidBody;


protected:

	glm::vec3 start;
	glm::vec3 end;
	float distance;
	float minDistance;
	
	float speed;
	float speedDelay;
	float respawnDelay;
	float respawnTimer;
	bool glassReceived;
	float drinkTime;
	float glassDelay;
	bool exitedFromPub;
	

//	BoundingRegion* clientBR;
};

