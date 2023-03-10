#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../graphics/text.h"

#include "basescene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

enum class MenuState
{
	TITLE,
	MAIN_MENU,
	NEW_GAME,
	CREDITS,
	RULES
};

struct Score
{
	std::string points;
};

class Menu : public BaseScene
{
public:
	Menu(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int scrHeight);

	virtual bool init();

	virtual void render();

	virtual void processInput(float dt);

	// update screen before each frame
	void update();

	virtual void cleanup();

	MenuState currentMenuState;

	bool buttonCentered(const char* label, float alignment = 0.5f);

	void textCentered(std::string text);
    void renderText();

private:
	TextRenderer textRenderer;
	Shader textShader;
	
};

