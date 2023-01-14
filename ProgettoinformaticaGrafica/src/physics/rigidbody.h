#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <string>

#define INSTANCE_DEAD	(unsigned char)0b00000001
#define INSTANCE_MOVED	(unsigned char)0b00000010
#define NO_COLLISION	(unsigned char)0b00000100

class RigidBody {
public:
	unsigned char state;

	// mass in kg
	float mass;

	// position in m
	glm::vec3 pos;
	// velocity in m/s
	glm::vec3 velocity;
	// acceleration in m/s^2
	glm::vec3 acceleration;

	// dimensions of object
	glm::vec3 size;

	// rotation in Euler angles
	glm::vec3 rot;

	// model matrix
	glm::mat4 model;
	glm::mat3 normalModel;

	std::string modelId;
	std::string instanceId;

	RigidBody();

	RigidBody(std::string modelId,
		glm::vec3 size = glm::vec3(1.0f),
		float mass = 1.0f, 
		glm::vec3 pos = glm::vec3(0.0f), 
		glm::vec3 rot = glm::vec3(0.0f));

	bool operator==(RigidBody rb);
	bool operator==(std::string id);
	
	void update(float dt);

	void applyForce(glm::vec3 force);
	void applyForce(glm::vec3 direction, float magnitude);

	void applyAcceleration(glm::vec3 acceleration);
	void applyAcceleration(glm::vec3 direction, float magnitude);

	void applyImpulse(glm::vec3 force, float dt);
	void applyImpulse(glm::vec3 direction, float magnitude, float dt);

	void transferEnergy(float joules, glm::vec3 direction);
};

#endif