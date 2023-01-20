#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "basescene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../graphics/text.h"

enum class TitleState
{
	DISPLAY,
	MAIN_MENU,
};

class Title : public BaseScene
{
public:
	Title(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int scrHeight);

	virtual bool init();

	virtual void render();

	virtual void processInput(float dt);

	// update screen before each frame
	void update();

	virtual void cleanup();

	void renderText();
	void textCentered(std::string text, int n);
	bool buttonCentered(const char* label, float alignment);

	TitleState currentTitleState;

private:
	TextRenderer textRenderer;
	Shader textShader;

	


};
#pragma once
