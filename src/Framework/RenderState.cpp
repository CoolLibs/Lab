#include "RenderState.h"

std::function<void()> RenderState::OnRenderAreaResized = []() {};
RectSizePos RenderState::m_Window;
RectSizePos RenderState::m_AvailableSpace;
RectSize RenderState::m_Export;
bool RenderState::m_bIsExporting = false;
bool RenderState::m_bConstrainInAppRenderAreaRatio = false;
float RenderState::m_inAppRenderAreaConstrainedRatio = 16.0f / 9.0f;

RectSize RenderState::Size() {
	if (m_bIsExporting)
		return m_Export;
	else
		return InAppRenderArea();
}

void RenderState::setIsExporting(bool bIsExporting) {
	m_bIsExporting = bIsExporting;
	OnRenderAreaResized();
}

void RenderState::setWindowSize(int width, int height) {
	m_Window.setSize(width, height);
	// No need to call OnRenderAreaResized() because it will be called by setAvailableSpaceSize() once the ImGui dockspace realizes its size has changed
}

void RenderState::setAvailableSpaceSize(int width, int height) {
	m_AvailableSpace.setSize(width, height);
	if (!m_bIsExporting)
		OnRenderAreaResized();
}

void RenderState::setExportSize(int width, int height) {
	m_Export.setSize(width, height);
	if (m_bIsExporting)
		OnRenderAreaResized();
}

RectSizePos RenderState::InAppRenderArea() {
	if (!m_bIsExporting && !m_bConstrainInAppRenderAreaRatio)
		return m_AvailableSpace;
	else {
		// Get aspect ratios
		float availableSpaceRatio = m_AvailableSpace.aspectRatio();
		float desiredAspectRatio;
		if (m_bIsExporting)
			desiredAspectRatio = m_Export.aspectRatio();
		else
			desiredAspectRatio = m_inAppRenderAreaConstrainedRatio;
		// Compute size
		RectSizePos res;
		if (desiredAspectRatio > availableSpaceRatio)
			res.setSize(m_AvailableSpace.width(), static_cast<int>(m_AvailableSpace.width() / desiredAspectRatio));
		else
			res.setSize(static_cast<int>(m_AvailableSpace.height() * desiredAspectRatio), m_AvailableSpace.height());
		// Compute position
		glm::ivec2 center = (m_AvailableSpace.topLeft() + m_AvailableSpace.botRight()) / 2;
		glm::ivec2 topLeft = center - res.size() / 2;
		res.setTopLeft(topLeft.x, topLeft.y);
		//
		return res;
	}
}

void RenderState::ImGuiConstrainInAppRenderAreaRatio() {
	if (ImGui::Checkbox("Constrain aspect ratio", &m_bConstrainInAppRenderAreaRatio)) {
		OnRenderAreaResized();
	}
	if (m_bConstrainInAppRenderAreaRatio) {
		if (ImGui::SliderFloat("Aspect ratio", &m_inAppRenderAreaConstrainedRatio, 0.5f, 2.0f)) {
			OnRenderAreaResized();
		}
	}
}