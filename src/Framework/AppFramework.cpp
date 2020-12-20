#include "AppFramework.h"

#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "imgui/imgui_internal.h"

#include "RenderState.h"
#include "Input.h"

AppFramework::AppFramework(GLWindow& glWindow, App& app)
	: m_glWindow(glWindow), m_app(app)
{
	Input::Initialize();
	RenderState::setRenderAreaResizedCallback([&app]() {app.onRenderAreaResized(); });
	onWindowMove();
	onWindowResize();
}

void AppFramework::onWindowMove() {
	int x, y;
	SDL_GetWindowPosition(m_glWindow.window, &x, &y);
	RenderState::setWindowTopLeft(x, y);
}

void AppFramework::onWindowResize() {
	int w, h;
	SDL_GetWindowSize(m_glWindow.window, &w, &h);
	RenderState::setWindowSize(w, h);
}

void AppFramework::updateAvailableRenderingSpaceSizeAndPos(ImGuiDockNode* node) {
	// Position
	RenderState::setAvailableSpaceTopLeft(
		static_cast<int>(node->Pos.x) - RenderState::getWindowTopLeft().x,
		static_cast<int>(node->Pos.y) - RenderState::getWindowTopLeft().y
	);
	// Size
	glm::ivec2 size = { static_cast<int>(node->Size.x), static_cast<int>(node->Size.y) };
	if (size.x != RenderState::getAvailableSpaceSize().x || size.y != RenderState::getAvailableSpaceSize().y) {
		RenderState::setAvailableSpaceSize(size.x, size.y);
	}
}

bool AppFramework::onEvent(const SDL_Event& e) {
	if (m_glWindow.checkForFullscreenToggles(e)) {
		onWindowResize();
		return false;
	}
	switch (e.type) {

	case SDL_WINDOWEVENT:
		switch (e.window.event) {

		case SDL_WINDOWEVENT_RESIZED:
			onWindowResize();
			return false;

		case SDL_WINDOWEVENT_CLOSE:
			if (e.window.windowID == SDL_GetWindowID(m_glWindow.window)) {
				closeApp();
				return false;
			}
			return false;

		case SDL_WINDOWEVENT_MOVED:
			onWindowMove();
			return false;

		default:
			return false;
		}

	case SDL_KEYDOWN:
		if (e.key.keysym.sym == 'h' && Input::KeyIsDown(SDL_SCANCODE_LCTRL)) {
			m_bShowUI = !m_bShowUI;
			return true;
		}
		return false;

	case SDL_QUIT:
		closeApp();
		return false;

	default:
		return false;
	}
}

void AppFramework::update() {
	// Events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (!onEvent(e))
			m_app.onEvent(e);
	}
	// Clear screen
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_glWindow.window);
	ImGui::NewFrame();
	ImGuiDockspace();
	// Actual application code
	m_app.update();
	if (m_bShowUI) {
		// Menu bar
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("RenderArea")) {
			RenderState::ImGuiConstrainInAppRenderAreaRatio();
			ImGui::EndMenu();
		}
		m_app.ImGuiMenus();
		ImGui::EndMainMenuBar();
		// Windows
		m_app.ImGuiWindows();
	}
	// Render ImGui
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	// End frame
	SDL_GL_SwapWindow(m_glWindow.window);
}

void AppFramework::ImGuiDockspace() {
	const ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground
									| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
									| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
									| ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	bool bopen = true;
	ImGui::Begin("MyMainDockSpace", &bopen, window_flags);
	ImGui::PopStyleVar(3);
	
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		updateAvailableRenderingSpaceSizeAndPos(ImGui::DockBuilderGetCentralNode(dockspace_id));
	}
	else
	{
		Log::Warn("Docking not enabled !");
	}
	ImGui::End();
}