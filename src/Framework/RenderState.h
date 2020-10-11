#pragma once

#include "Utility/RectSizePos.h"

class RenderState {
public:
	// Size that should be used for rendering
	static RectSize Size();
	static inline bool IsExporting() { return m_bIsExporting; }

private:
	friend class AppFramework;
	friend class Renderer;
	friend class Input;
	// Size and position that should be used for showing rendered images inside the window
	static RectSizePos InAppRenderArea();
	inline static glm::ivec2 SwapYConvention(const glm::ivec2& p) { return { p.x, m_Window.height() - p.y }; }

	static void setIsExporting(bool bIsExporting);
	static void setExportSize        (int width, int height);
	static void setWindowSize        (int width, int height);
	static void setAvailableSpaceSize(int width, int height);
	static inline void setWindowTopLeft        (int x, int y) { m_Window .setTopLeft(x, y); }
	static inline void setAvailableSpaceTopLeft(int x, int y) { m_AvailableSpace.setTopLeft(x, y); }

	static inline const glm::ivec2& getExportSize()         { return m_Export.size(); }
	static inline const glm::ivec2& getWindowSize()         { return m_Window.size(); }
	static inline const glm::ivec2& getAvailableSpaceSize() { return m_AvailableSpace.size(); }
	static inline       glm::ivec2  getWindowTopLeft()      { return m_Window.topLeft(); }

	static void ImGuiConstrainInAppRenderAreaRatio();

	static inline void setRenderAreaResizedCallback(const std::function<void()>& callback) { OnRenderAreaResized = callback; }

private:
	static std::function<void()> OnRenderAreaResized; // Function pointer to be used as a callback

	static RectSizePos m_Window;         // Size of the window, and it's position inside the screen
	static RectSizePos m_AvailableSpace; // Size of the available space inside the window (excludes the docked ImGui windows), and its position inside the window
	static RectSize m_Export;            // Size of the exported images

	static bool m_bIsExporting; // Owned by RenderState because it needs to know it when deciding what the rendered size should be

	static bool m_bConstrainInAppRenderAreaRatio;
	static float m_inAppRenderAreaConstrainedRatio;
};