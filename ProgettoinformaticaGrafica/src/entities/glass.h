#pragma once

#include "entitybase.h"
#include "../io/mouse.h"
#include "../io/keyboard.h"
#include "../physics/environment.h"
#include "../physics/rigidbody.h"
#include "../algorithms/states.hpp"
#include "../algorithms/trie.hpp"


#define DEBUG 0

class Glass  //public EntityBase
{
public:

	Glass(const std::string& name, const std::string& modelId, Scene* scene);

	virtual void init(glm::vec3 size, float mass, glm::vec3 pos);
	virtual void update(double dt) = 0;

	const std::string name;
	const std::string modelId;
	Scene* scene;

	RigidBody* rigidBody;
	

	bool full;
	bool drank;
	float timeToFill;
	float timeBetween;

	float speedToGo;
	float speedToReturn;
	bool exitedFromPub;
	bool collected;

	//BoundingRegion* glassBR;
};

