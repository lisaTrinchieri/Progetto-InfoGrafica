/*
	CODE
	ENGINE (Scene)
	OPENGL
	GPU
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <stack>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>      // all the GLSL trigonometric functions: radians, cos, asin, etc.

#include <ft2build.h>
#include FT_FREETYPE_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/model.h"
#include "graphics/light.h"
#include "graphics/text.h"

#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/models/box.hpp"
#include "graphics/models/sphere.hpp"

#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/camera.h"

#include "algorithms/states.hpp"

#include "scenes/title.h"
#include "scenes/menu.h"
#include "scenes/scene.h"

#include "entities/player.h"
#include "entities/client.h"
#include "entities/glass.h"


#include "physics/rigidbody.h"
#include "physics/environment.h"
#include <glm/gtx/norm.hpp>

struct Path {
	glm::vec3 start;
	glm::vec3 end;
};


Camera cam;
void processInput(double dt, Player* scene);

std::ostream& operator <<(std::ostream& out, const glm::vec3& v) {
	out << "glm::vec3(" << v.x << ", " << v.y
		<< ", " << v.z << ")";
	return out;
}

int main()
{
        Menu menu(3, 3, "TAPPER-Menu", 800, 600);
		if (!menu.init())
		{
			std::cout << "Could not initialize Menu\n";
			return -1;
		}

		while (!menu.shouldClose() && !(menu.currentMenuState == MenuState::NEW_GAME))
		{
			// process input
			menu.processInput(0.0f);

			menu.render();
			menu.update();

			menu.newFrame();

			
		}


		if (!menu.shouldClose())
		{

			Scene scene(3, 3, "Tapper", 800, 600);

			//GameScene scene(3, 3, "Progetto Informatica Grafica", 800, 600);
			if (!scene.init())
			{
				std::cout << "Could not init game scene" << std::endl;
				glfwTerminate();
				return -1;
			}


			scene.activeCamera = 0;

			/* SHADERS */
			Shader shader("assets/shaders/instanced_vs.glsl", "assets/shaders/core_fs.glsl");
			Shader lampShader("assets/shaders/instanced_vs.glsl", "assets/shaders/lamp_fs.glsl");
			Shader boxShader("assets/shaders/box_vs.glsl", "assets/shaders/box_fs.glsl");

			/* MODELS */

			Lamp lamp;
			scene.registerModel(&lamp);

			Model room("room", BoundTypes::AABB, CONST_INSTANCES);
			room.loadModel("assets/models/room/room.obj");
			scene.registerModel(&room);
			scene.generateInstance(room.getId(), glm::vec3(1.0f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(180.0f, -90.0f, -90.0f));
			//room.setMaterial(Material::grey);

			Model book("book", BoundTypes::AABB, CONST_INSTANCES);
			book.loadModel("assets/models/book/books.obj");
			scene.registerModel(&book);
			scene.generateInstance(book.getId(), glm::vec3(1.0f), 1.0f, glm::vec3(0.0f, -9.0f, 0.0f), glm::vec3(0.0f, -90.0f, -90.0f));


			Box box;
			box.init();

			Player* player = new Player("", "Waiter", &scene);
			// scene.addEntity(player);



			scene.loadModels();

			// LIGHTS
			DirLight dirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
			scene.dirLight = &dirLight;

			glm::vec3 pointLightPositions[] = {
				glm::vec3(0.7f,  0.2f,  2.0f),
				glm::vec3(2.3f, -3.3f, -4.0f),
				glm::vec3(-4.0f,  2.0f, -12.0f),
				glm::vec3(0.0f,  0.0f, -3.0f)
			};

			glm::vec4 ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
			glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
			glm::vec4 specular = glm::vec4(1.0f);
			float k0 = 1.0f;
			float k1 = 0.09f;
			float k2 = 0.032f;

			PointLight pointLights[4];

			for (unsigned int i = 0; i < 4; i++) {
				pointLights[i] = PointLight(
					pointLightPositions[i],
					k0, k1, k2,
					ambient, diffuse, specular
				);
				scene.generateInstance(lamp.getId(), glm::vec3(0.25f), 0.25f, pointLightPositions[i]);
				scene.pointLights.push_back(&pointLights[i]);
				States::activate(&scene.activePointLights, i);
			}

				SpotLight spotLight(
					cam.cameraPos, cam.cameraFront,
					glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
					1.0f, 0.07f, 0.032f,
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f));
				scene.spotLights.push_back(&spotLight);
				scene.activeSpotLights = 1;	// 0b00000001  

			scene.initInstances();

			scene.setBox(&box);

			double dt = 0.0f; // tme btwn frames
			double lastFrame = 0.0f; // time of last frame
			glfwSetTime(0.0f);
			while (!scene.shouldClose() && scene.state != GameState::GAME_OVER)
			{
				double currentTime = glfwGetTime();
				dt = currentTime - lastFrame;
				lastFrame = lastFrame + dt;

				scene.update(dt);

				// process input
				scene.processInput(dt);
				processInput(dt, player);

				//player.render(dt);
				scene.render();

				//scene.renderShader(shader);
				//scene.renderInstances(room.getId(), shader);

				scene.renderShader(shader);
				scene.renderInstances(book.getId(), shader);


				scene.renderText();

				// send new frame to window
				scene.newFrame();
				scene.clearDeadInstances();
			}

			delete player;

		}
		return 0;
	
}

void processInput(double dt, Player* player)
{
	
	if (Keyboard::keyWentDown(GLFW_KEY_P))
	{
		std::cout << cam.cameraPos << std::endl;
	}
	


	if (Keyboard::keyWentDown(GLFW_KEY_1))
	{
		//std::cout << "Launch axe\n";
		/*
		if (scene)
		{
			if (scene->getAxes() > 0)
			{
				RigidBody* rb = scene->generateInstance(axe.id, glm::vec3(.50f), 1.0f, cam.cameraPos, glm::vec3(0.0, glm::radians(-cam.yaw + 90.0), 0.0));
				scene->removeAxe();
				if (rb) {
					// instance generated
					rb->transferEnergy(100.0f, cam.cameraFront);
					rb->applyAcceleration(Environment::gravitationalAcceleration);
				}
			}
			else
			{
				std::cout << "not enough axes\n";
			}

		}
		
		if (player)
		{
			//player->throwAxe();
		}

   */
	} 

}