#include "App.h"

#include "Framework/RenderState.h"
#include "Framework/Input.h"
#include "Framework/Time.h"

App::App()
	: m_shaderWatcher("myShaders/ArtOfCode-StartingPoint.frag", [this](const char* path) { m_shader.compile("shaders/fullscreen.vert", path); })
{
	RenderState::setRenderAreaResizedCallback([this]() {m_renderer.onRenderAreaResized(); });
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Please note that the blending is WRONG for the alpha channel (but it doesn't matter in most cases) The correct call would be glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE) a.k.a. newAlpha = srcAlpha + dstAlpha - srcAlpha*dstAlpha
	RenderState::setExportSize(1920, 1080);
}

void App::render() {
	m_renderer.begin();
	{
		glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_shader.bind();
		m_shader.setUniform1f("uAspectRatio", RenderState::Size().aspectRatio());
		m_shader.setUniform3f("uCamX", m_camera.xAxis());
		m_shader.setUniform3f("uCamY", m_camera.yAxis());
		m_shader.setUniform3f("uCamZ", m_camera.zAxis());
		m_shader.setUniform3f("uCamPos", m_camera.position());
		m_shader.setUniform1f("uFocalLength", m_camera.focalLength());
		m_shader.setUniform1f("uTime", Time::time());
		m_renderer.dummyDrawCallForFullscreen();
	}
	m_renderer.end();
}

void App::update() {
	m_shaderWatcher.update();
	m_camera.update();
	render();
	Time::Update();
}

void App::ImGuiWindows() {
	ImGui::Begin("Time");
	Time::ImGui();
	ImGui::End();
#ifndef NDEBUG
	if (m_bShow_Debug) {
		ImGui::Begin("Debug", &m_bShow_Debug);
		ImGui::Text("Application average %.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Text("Render Area Size : %d %d", RenderState::Size().width(), RenderState::Size().height());
		ImGui::Text("Mouse Position in Render Area : %d %d", Input::MouseInPixels().x, Input::MouseInPixels().y);
		ImGui::Text("Mouse Position Normalized : %.2f %.2f", Input::MouseInNormalizedRatioSpace().x, Input::MouseInNormalizedRatioSpace().y);
		ImGui::Text("Camera Transform matrix :");
		glm::mat4 m = m_camera.transformMatrix();
		ImGui::Text("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f",
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		);
		ImGui::ColorEdit3("Background Color", glm::value_ptr(m_bgColor));
		ImGui::Checkbox("Show Demo Window", &m_bShow_ImGuiDemo);
		ImGui::End();
	}
	if (m_bShow_ImGuiDemo) // Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&m_bShow_ImGuiDemo);
#endif
}

void App::ImGuiMenus() {
	if (ImGui::BeginMenu("Windows")) {
#ifndef NDEBUG
		ImGui::Separator();
		ImGui::Checkbox("Debug", &m_bShow_Debug);
#endif
		ImGui::EndMenu();
	}
}

void App::onEvent(const SDL_Event& e) {
	if (!RenderState::IsExporting()) {
		switch (e.type) {

		case SDL_MOUSEMOTION:
			if (!ImGui::GetIO().WantCaptureMouse) {

			}
			break;

		case SDL_MOUSEWHEEL:
			m_camera.onWheelScroll(e.wheel.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (!ImGui::GetIO().WantCaptureMouse) {
				switch(e.button.button) {
				case SDL_BUTTON_LEFT:
					m_camera.onWheelDown();
					break;
				case SDL_BUTTON_RIGHT:
					break;
				case SDL_BUTTON_MIDDLE:
					m_camera.onWheelDown();
					break;
				}
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (!ImGui::GetIO().WantCaptureMouse) {
				switch (e.button.button) {
				case SDL_BUTTON_LEFT:
					m_camera.onWheelUp();
					break;
				case SDL_BUTTON_RIGHT:
					break;
				case SDL_BUTTON_MIDDLE:
					m_camera.onWheelUp();
					break;
				}
			}
			break;

		case SDL_KEYDOWN:
			if (!ImGui::GetIO().WantTextInput) {
				if (e.key.keysym.sym == 's' && Input::KeyIsDown(SDL_SCANCODE_LCTRL)) {
					m_exporter.exportFrame(m_renderer.renderBuffer(), [this]() {render();});
				}
			}
			break;

		case SDL_KEYUP:
			if (!ImGui::GetIO().WantTextInput) {

			}
			break;

		default:
			break;
		}
	}
}

void App::onRenderAreaResized() {
	m_renderer.onRenderAreaResized();
}