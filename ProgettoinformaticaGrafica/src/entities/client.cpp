#include "client.h"
#include "glass.h"



Client::Client(const std::string& name, const std::string& modelId, Scene* scene)
	: name(name), modelId(modelId), scene(scene),
	distance(0.f)
	minDistance(1.5f);
    respawnDelay(5.0f),
    respawnTimer(respawnDelay),
    speed(1),
    speedDelay(3.0f)
    drinkTime(2.0f);
    glassDelay(drinkTime);
    exitedFromPub(false);
    glassReceived(false);
{

}

void Client::init(glm::vec3 size, float mass, glm::vec3 pos)
{
	rigidBody = scene->generateInstance(modelId, size, mass, pos);
}


void Client::setPath(glm::vec3 start, glm::vec3 end)
{
	this->start = start;
	this->end = end;

	rigidBody -> pos = 
	this->speed = (speed > maxSpeed) ? maxSpeed : speed;
	rigidBody->velocity = glm::normalize(end - start) * speed;
	glm::vec3 v = rigidBody->velocity;
	float yaw = atan2(rigidBody->velocity.x, rigidBody->velocity.z);
	rigidBody->rot = glm::vec3(0.0f, glm::half_pi<float>() - yaw, 0.0f);
}

void Client::init(glm::vec3 size, float mass, glm::vec3 pos)
{
	EntityBase::init(size, mass, pos);
	distance = 0.f;
	glassReceived=false;
}

void Client::update(double dt)
{
	if (rigidBody)
	{

		if (scene)
		{  
			this->distance = glm::length(start - rigidBody->pos);

			if (glassReceived)
			{
				float vel = glass->speedToGo;
				rigidBody->velocity = vel;


				if (distance <= minDistance)
				{
					this->exitedFromPub = true;
					glassObj->exitedFromPub = true;

				}
				else
				{

					setPath(rigidBody->pos - end);

				}




				//std::cout << "Player nearby\n";
				if (shootTimer <= 0.0f)
				{
					
					RigidBody* projectile_rb = scene->generateInstance("projectile", glm::vec3(.1f), 0.50f, rigidBody->pos);
					if (projectile_rb) {
						// instance generated
						projectile_rb->transferEnergy(80.0f, glm::normalize(camera->cameraPos + glm::vec3(0.0f, 0.10f, 0.0f) - rigidBody->pos));
						projectile_rb->applyAcceleration(Environment::gravitationalAcceleration);
					}
					shootTimer = shootDelay;
				}
				else
				{
					shootTimer -= dt;
				}
			}
			else {
				//std::cout << "Player too far\n";
			}
		}
	}
	else
	{
		// if the enemy is alive
		// decrement respawn time
		respawnTimer -= dt;
		if (respawnTimer <= 0)
		{
			// respawn
			respawnTimer = respawnDelay;
			init(glm::vec3(1.0f), 1.0f, start);
			speed++;
			setPath(end, start);
		}
	}

}
