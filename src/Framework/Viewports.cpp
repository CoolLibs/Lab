#include "Viewports.h"

RectSizePos Viewports::m_Window;
RectSizePos Viewports::m_AvailableAppView;
RectSize Viewports::m_Export;

bool Viewports::m_bIsExporting = false;

bool Viewports::m_bConstrainAppViewRatio = false;
float Viewports::m_appViewConstrainedRatio = 16.0f / 9.0f;

std::function<void()> Viewports::OnRenderAreaResized = [](){};

RectSize Viewports::Size() {
	if (m_bIsExporting)
		return m_Export;
	else
		return AppView();
}

void Viewports::setIsExporting(bool bIsExporting) {
	m_bIsExporting = bIsExporting;
	OnRenderAreaResized();
}

void Viewports::setWindowSize(int width, int height) {
	m_Window.setSize(width, height);
}

void Viewports::setAvailableAppViewSize(int width, int height) {
	m_AvailableAppView.setSize(width, height);
	if (!m_bIsExporting)
		OnRenderAreaResized();
}

void Viewports::setExportSize(int width, int height) {
	m_Export.setSize(width, height);
}

RectSizePos Viewports::AppView() {
	if (!m_bIsExporting && !m_bConstrainAppViewRatio)
		return m_AvailableAppView;
	else {
		// Get aspect ratios
		float appViewRatio = m_AvailableAppView.aspectRatio();
		float aspectRatio;
		if (m_bIsExporting)
			aspectRatio = m_Export.aspectRatio();
		else
			aspectRatio = m_appViewConstrainedRatio;
		// Compute size
		RectSizePos res;
		if (aspectRatio > appViewRatio)
			res.setSize(m_AvailableAppView.width(), static_cast<int>(m_AvailableAppView.width() / aspectRatio));
		else
			res.setSize(static_cast<int>(m_AvailableAppView.height() * aspectRatio), m_AvailableAppView.height());
		// Compute position
		glm::ivec2 center = (m_AvailableAppView.topLeft() + m_AvailableAppView.botRight()) / 2;
		glm::ivec2 topLeft = center - res.size() / 2;
		res.setTopLeft(topLeft.x, topLeft.y);
		//
		return res;
	}
}

void Viewports::ImGuiConstrainAppViewRatio() {
	if (ImGui::Checkbox("Constrain aspect ratio", &m_bConstrainAppViewRatio)) {
		OnRenderAreaResized();
	}
	if (m_bConstrainAppViewRatio) {
		if (ImGui::SliderFloat("Aspect ratio", &m_appViewConstrainedRatio, 0.5f, 2.0f)) {
			OnRenderAreaResized();
		}
	}
}