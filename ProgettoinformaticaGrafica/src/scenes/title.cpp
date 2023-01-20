#include "title.h"

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

Title::Title(int glfwVersionMajor, int glfwVersionMinor, const char* title, unsigned int scrWidth, unsigned int scrHeight)
	:BaseScene(glfwVersionMajor, glfwVersionMinor, title, scrWidth, scrHeight),
	textRenderer(TextRenderer("assets/fonts/comic.ttf", 60))
{
//	sceneType = SceneType::TITLE;
	currentTitleState = TitleState::DISPLAY;
		
}

void Title::renderText()
{

	textRenderer.render(textShader, "TAPPER", 40.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.5f));
	

}

bool Title::init() {
	// init window
	if (BaseScene::init()) {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		textRenderer.init();
		textShader = Shader("assets/shaders/glyph_vs.glsl", "assets/shaders/glyph_fs.glsl");
		//ImGui::StyleColorsLight();
		if (ImGui_ImplGlfw_InitForOpenGL(window, true)) {
			return ImGui_ImplOpenGL3_Init("#version 130");
		}
	}
	return false;
}

void Title::render()
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
	//ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5, 0.5);
	colors[ImGuiCol_WindowBg] = ImVec4(0.6f, 0.30f, 0.00f, 1.0f);


	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	textCentered("TAPPER", 10);
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
	ImGui::Text("");
	ImGui::Text("");
	textCentered("...Tap here to go on...", 2);
	if (buttonCentered("START", 0.52f))
	{
		currentTitleState = TitleState :: MAIN_MENU;
	}
	ImGui::End();

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
}

void Title::update()
{
	glClearColor(background.r, background.g, background.b, background.a);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Title::cleanup()
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

bool Title::buttonCentered(const char* label, float alignment)
{
	//ImGuiStyle& style = ImGui::GetStyle();

	//float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	//float avail = ImGui::GetContentRegionAvail().x;

	float off = (BaseScene::scrWidth - BaseScene::scrWidth / 5.0f) * alignment;
	if (off > 0.0f)
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);

	return ImGui::Button(label, ImVec2(BaseScene::scrWidth / 6, BaseScene::scrHeight / 12));
}


void Title::textCentered(std::string text, int n) {
	float font_size = ImGui::GetFontSize() * text.size() / 2;
	ImGui::SameLine(
		ImGui::GetWindowSize().x / 2 -
		font_size + (font_size / 2)
	);

	ImGui::Text(text.c_str());
	ImGui::Text("");
}

void Title::processInput(float dt)
{
	if (Keyboard::keyWentDown(GLFW_KEY_ESCAPE)) {
			setShouldClose(true);
		


	}
	

}