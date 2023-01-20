﻿#include "scene.h"

#include <iostream>
#include <fstream>

/*
	constructor
*/

Scene::Scene(int glfwVersionMajor, int glfwVersionMinor,
	const char* title, unsigned int scrWidth, unsigned int scrHeight)
	: BaseScene(glfwVersionMajor, glfwVersionMinor, title, scrWidth, scrHeight),
	state(GameState::PLAYING),
	activeCamera(-1),
	//activePointLights(0), activeSpotLights(0),
	points(0),
	textRenderer(TextRenderer("assets/fonts/comic.ttf", 48))
{ }

bool Scene::init()
{
	if (BaseScene::init()) {
		
		/*
		*	Text Rendering Library
		*/

		textRenderer.init();
		textShader = Shader("assets/shaders/glyph_vs.glsl", "assets/shaders/glyph_fs.glsl");
		return true;
	}
	return false;
}

/*
	main loop methods
*/

void Scene::newFrame()
{
	// clear old positions and sizes
	box->rotations.clear();
	box->positions.clear();
	box->sizes.clear();

	BaseScene::newFrame();
}

void Scene::update(double dt)
{
	BaseScene::update();

	updateInstancies(dt);

	updateEntities(dt);

	//playerBR->center = player->playerPos;

	updateBoundings(dt);

	checkCollision(dt);
}

void Scene::checkCollision(double dt)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		BoundingRegion br = objects[i];
		if (!States::isActive(&instances[br.instance->instanceId]->state, INSTANCE_DEAD) && !States::isActive(&instances[br.instance->instanceId]->state, (unsigned char)NO_COLLISION))
		{
			if (States::isActive(&br.instance->state, INSTANCE_MOVED))
			{
				br.transform();
				if (br.instance->modelId == "glass")
				{
					handleGlassCollision(br);
				}

			}
		}
	} 
}

void Scene::handleGlassCollision(BoundingRegion& br)
{
	/*for (int j = 0; j < objects.size(); ++j)
	{
		BoundingRegion other = objects[j];
		if (br.instance->instanceId != other.instance->instanceId && !States::isActive(&instances[other.instance->instanceId]->state, INSTANCE_DEAD))
		{
			other.transform();
			//std::cout << "Checking instance " << br.instance->instanceId << " of " << br.instance->modelId << " with instance" << other.instance->instanceId <<" of " << other.instance->modelId << std::endl;
			if (br.intersectsWith(other))
			{
			/*	if (other.instance->modelId == "player" && (br.instance->drank == true))
				{
					glassesCollected++;
					player->collectGlass();
				}
				else if (other.instance->modelId == "client"  && (br.instance->full == true))
				{
					glassesServed++;

				
				}
            
					//std::cout << "Instance of model " << br.instance->modelId << " collides with instance of " << other.instance->modelId << std::endl;
					markForDeletion(br.instance->instanceId);
					break; 
				}

			}
		} */
}



// process input
void Scene::processInput(float dt)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		setShouldClose(true);
	}
	if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		state = GameState::PAUSE;
	}
	if (activeCamera != -1 )
	{

		// set matrices
		//view = cameras[activeCamera]->getViewMatrix();
		//projection = glm::perspective(
		//	glm::radians(cameras[activeCamera]->getZoom()),	// FOV
		//	(float)scrWidth / (float)scrHeight,					// aspect ratio
		//	0.01f, 100.0f										// near and far bounds
		//);

		// set pos at end
	//	cameraPos = cameras[activeCamera]->cameraPos;

		if (States::isIndexActive(&activeSpotLights, 0))
		{
			spotLights[0]->position = cameraPos;
			//spotLights[0]->direction = cameras[activeCamera]->cameraFront;
		}
	
	}
}

// set uniform shader varaibles (lighting, etc)
void Scene::renderShader(Shader shader, bool applyLighting)
{
	// activate shader
	shader.activate();

	// set camera values
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.set3Float("viewPos", cameraPos);

	// lighting
	if (applyLighting)
	{
		// point lights
		unsigned int noLights = pointLights.size();
		unsigned int noActiveLights = 0;
		for (unsigned int i = 0; i < noLights; i++)
		{
			if (States::isActive(&activePointLights, i))
			{
				// i'th light is active
				pointLights[i]->render(shader, noActiveLights);
				noActiveLights++;
			}
		}
		shader.setInt("noPointLights", noActiveLights);

		// spot lights
		noLights = spotLights.size();
		noActiveLights = 0;
		for (unsigned int i = 0; i < noLights; i++)
		{
			if (States::isActive(&activeSpotLights, i))
			{
				// i'th spot light active
				spotLights[i]->render(shader, noActiveLights);
				noActiveLights++;
			}
		}
		shader.setInt("noSpotLights", noActiveLights);

		// directional light
		dirLight->render(shader);
		
	}
}

void Scene::renderInstances(std::string modelId, Shader shader)
{
	if (models[modelId])
	{
		models[modelId]->render(shader);
	}
	else
	{
		std::cerr << "Cannot find model with " << modelId << " id\n";
	}
}

void Scene::render()
{
	std::cerr << "Render function not defined without shader input\n";
}

void Scene::renderText()
{
	textRenderer.render(textShader, "Points: " + std::to_string(points), 40.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.5f));
	textRenderer.render(textShader, "Clients served: " + std::to_string(clientsServed), 40.0f, 530.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.5f));
	textRenderer.render(textShader, "Lives: ", 540.0f, 490.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.5f));
	for (unsigned int i = 0; i < lives; ++i) {
		textRenderer.render(textShader, "<3", 625.0f + i * 25.0f, 490.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.5f));
	}

}

void Scene::onGameOver()
{
	
//	out.close();
	state = GameState::GAME_OVER;
}

//void Scene::addPlayer(Player* entity)
//{
//	player = entity;
//}

/*void Scene::addClient(Client* entity)
{
	clients.push_back(entity);

}

void Scene::addGlass(Glass* entity)
{
	glasses.push_back(entity);
} */

void Scene::registerModel(Model* model)
{
	if (models.count(model->getId()) == 0)
	{
		//std::cout << "Registering model " << model->id << std::endl;
		models.insert(std::pair<std::string, Model*>(model->getId(), model));
	}

}


void Scene::setBox(Box* box)
{
	this->box = box;
}

RigidBody* Scene::generateInstance(std::string modelId, glm::vec3 size, float mass, glm::vec3 pos, glm::vec3 rot)
{
	RigidBody* rb = models[modelId]->generateInstance(size, mass, pos, rot);

	if (rb)
	{
		//std::string id = generateId();
		std::string id = "";
		rb->instanceId = id;
		instances.insert(id, rb);
		addToPending(rb);
		return rb;
	}

	return nullptr;
}
void Scene::initInstances()
{
	for (std::map<std::string, Model*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		Model* model = it->second;
		model->initInstances();
	}
}

void Scene::loadModels()
{
	for (std::map<std::string, Model*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		Model* model = it->second;
		model->init();
	}
}

void Scene::removeInstance(std::string instanceId)
{
	// remove all locations
	//	- Scene::instances
	//	- Model::instances

	RigidBody* instance = instances[instanceId];
	std::string targetModel = instance->modelId;

	models[targetModel]->removeInstance(instanceId);

	/*for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i].instance->instanceId == instanceId)
		{
			//std::cout << "deleting br " << objects[i].instance->instanceId << std::endl;
			for (int j = 0; j < models[targetModel]->boundingRegions.size(); ++j)
			{
				objects.erase(objects.begin() + i);
			}

			break;
		}
	} */
	//delete instances[instanceId];
	instances[instanceId] = nullptr;
	instances.erase(instanceId);
}

void Scene::markForDeletion(std::string instanceId)
{
	RigidBody* instance = instances[instanceId];

	States::activate(&instance->state, INSTANCE_DEAD);
	//instancesToDelete.push_back(instance);
}

void Scene::clearDeadInstances()
{
	for (RigidBody* rb : instancesToDelete)
	{
		
			std::cout << "Deleting " << rb->instanceId << " of model " << rb->modelId << '\n';
			std::cout << "[x: " << rb->pos.x << ", y: " << rb->pos.y
				<< ", z:" << rb->pos.z << "]\n";
		

		removeInstance(rb->instanceId);
		rb = nullptr;
	}
	instancesToDelete.clear(); 
}

void Scene::addToPending(RigidBody* instance)
{
	for (BoundingRegion br : models[instance->modelId]->boundingRegions)
	{
		br.instance = instance;
		br.transform();
		if (!States::isActive(&instance->state, (unsigned char)NO_COLLISION))
		{
			objects.push_back(br);
		}
	}
}

const int Scene::getPoints() const
{
	return points;
}

void Scene::cleanup() {
	for (auto it = models.begin(); it != models.end(); ++it)
	{
		Model* model = it->second;
		model->cleanup();
		free(model);
		model = nullptr;
	}

	if (BaseScene::instances == 1)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}


void Scene::updateEntities(double dt)
{
	/*for (int i = 0; i < clients.size(); ++i)
	{
		clients[i]->update(dt);
	}

	for (int i = 0; i < glasses.size(); ++i)
	{
		glasses[i]->update(dt);
	} */
}

void Scene::updateBoundings(double dt)
{
	for (BoundingRegion br : objects)
	{
		if (!States::isActive(&br.instance->state, (unsigned char)NO_COLLISION))
		{
			if (States::isActive(&br.instance->state, INSTANCE_MOVED))
			{
				br.transform();
				//std::cout << br.instance->modelId << std::endl;
			}

			box->rotations.push_back(br.instance->rot);
			box->positions.push_back(br.calculateCenter());
			box->sizes.push_back(br.calculateDimensions());

		} 
	}
	//box->positions.push_back(ExitBR->calculateCenter());
	//box->sizes.push_back(ExitBR->calculateDimensions());
}

void Scene::updateInstancies(double dt)
{
	for (std::map<std::string, Model*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		Model* model = it->second;
		if (model->getNoInstances() > 0)
		{
			model->update(dt);
		}
	}
}

/*
void Scene::removeGlass()
{
	glass--;
}

const unsigned int Scene::getGlass() const
{
	return glasses;
}

void Scene::addGlass(const unsigned int glasses)
{
	this->glasses += glasses;
}
*/
