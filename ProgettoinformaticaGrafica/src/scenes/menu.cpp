#include "menu.h"

#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <algorithm>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Menu::Menu(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int scrHeight)
	:BaseScene(glfwVersionMajor, glfwVersionMinor, title, scrWidth, scrHeight), textRenderer(TextRenderer("assets/fonts/comic.ttf", 48))
{
	sceneType = SceneType::MENU;
	currentMenuState = MenuState::TITLE;

}

bool Menu::init() {
	// init window
	if (BaseScene::init()) {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//ImGui::StyleColorsLight();
			if (ImGui_ImplGlfw_InitForOpenGL(window, true)) {
				return ImGui_ImplOpenGL3_Init("#version 130");
			}
	}

	textRenderer.init();
	textShader = Shader("assets/shaders/glyph_vs.glsl", "assets/shaders/glyph_fs.glsl");

	return false;
}

void Menu::render()
{
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	// We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
	// Based on your use case you may want one of the other.
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::Begin(title, NULL, window_flags);                          // Create a window  and append into it

	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::Checkbox("Another Window", &show_another_window);

	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	char buf1[64] = "";

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGui::GetStyle().WindowRounding = 0.0f;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
	colors[ImGuiCol_WindowBg] = ImVec4(0.6f, 0.30f, 0.00f, 0.9f);

	ImGui::Text("");
	ImGui::Text("");

	switch (currentMenuState)
	{
	case MenuState :: TITLE:
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		//textRenderer.render(textShader, "TAPPER", 140.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		textCentered("...Tap here to go on...");
		if (buttonCentered("START", 0.5f))
		{
			currentMenuState = MenuState::MAIN_MENU;
		}
		break;
	case MenuState::MAIN_MENU:
		if (buttonCentered(" PLAY "))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			currentMenuState = MenuState::NEW_GAME;
		}
		if (buttonCentered(" Rules "))
		{
			currentMenuState = MenuState::RULES;
		}
		if (buttonCentered(" Credits "))
		{
			currentMenuState = MenuState::CREDITS;
		}
		if (buttonCentered("  EXIT "))
		{
			setShouldClose(true);
		}
		break;
	case MenuState::CREDITS:
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		textCentered("Informatica Grafica");
	    textCentered("Anno Accademico 2022 / 23");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		textCentered("TAPPER");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text(" Progetto d'esame di : ");
		ImGui::BulletText(" Claudia Gasparre - ");
		ImGui::BulletText(" Emanuele Marcantonio - ");
		ImGui::BulletText(" Lisa Trinchieri - ");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		textCentered("Press here to return to main menu...");
		if (buttonCentered(" <- BACK ", 0.5f))
		{
			currentMenuState = MenuState::MAIN_MENU;
		}
		break;
	case MenuState::RULES:
		textCentered("Esci dal labirinto evitando o sconfiggendo i mostri.");
		ImGui::Text("");
		textCentered("Attenzione pero'! Il pericolo e' dietro l'angolo");
		textCentered("e le risorse a disposizione sono limitate.");
		ImGui::Text("");
		textCentered("Sconfiggi i mostri per ricaricare munizioni");
		textCentered("Raccogli le caramelle per ottenere vite bonus e munizioni");
		ImGui::Text("");
		textCentered("Esci dal labirinto sconfiggendo piu' nemici possibile per entrare nella leaderboard!");
		ImGui::Text("");
		ImGui::Text("");
		ImGui::Text("");
		textCentered("Press here to return to main menu...");
		if (buttonCentered(" <- BACK ", 0.5f))
		{
			currentMenuState = MenuState::MAIN_MENU;
		}
		break;
	default:
		break;
	}

	ImGui::End();

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
}

void Menu::update()
{
	glClearColor(background.r, background.g, background.b, background.a);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::renderText()
{
	textRenderer.render(textShader, "TAPPER", 40.0f, 530.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.5f));
	

}

void Menu::cleanup()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	if (BaseScene::instances == 1) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	
}

bool Menu::buttonCentered(const char* label, float alignment)
{
	//ImGuiStyle& style = ImGui::GetStyle();

	//float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	//float avail = ImGui::GetContentRegionAvail().x;

	float off = (BaseScene::scrWidth - BaseScene::scrWidth / 5.0f) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);

	return ImGui::Button(label, ImVec2(BaseScene::scrWidth / 5, BaseScene::scrHeight / 10));
		//ImGui::ImageButton(&txtid->"images/whiteButton.png", ImVec2(BaseScene::scrWidth / 5, BaseScene::scrHeight / 10), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
}

void Menu::textCentered(std::string text) {
	float font_size = ImGui::GetFontSize() * text.size() / 2;
	ImGui::SameLine(
		ImGui::GetWindowSize().x / 2 -
		font_size + (font_size / 2)
	);

	ImGui::Text(text.c_str());
	ImGui::Text("");
}

void Menu::processInput(float dt)
{
	if (Keyboard::keyWentDown(GLFW_KEY_ESCAPE)) {
		switch (currentMenuState)
		{
		case MenuState::TITLE:
			setShouldClose(true);
			break;
		case MenuState::MAIN_MENU:
			setShouldClose(true);
			break;
		case MenuState::NEW_GAME:
			break;
	/*	case MenuState::CREDITS:
			currentMenuState = MenuState::MAIN_MENU;
			break;
		case MenuState::RULES:
			currentMenuState = MenuState::MAIN_MENU;
			break;  */
		default:
			setShouldClose(true);
			break;
		}
	}

}