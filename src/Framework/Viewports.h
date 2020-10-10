#pragma once

#include "RectSizePos.h"

class Viewports {
	friend class AppFramework;
public:
	static RectSize Size(); // Size that should be used for rendering
	static inline bool IsExporting() { return m_bIsExporting; }

	static RectSizePos AppView(); // Size of the available viewing area and its position inside the window // TODO make me private

private:
	inline static glm::ivec2 SwapYConvention(const glm::ivec2& p) { return { p.x, m_Window.height() - p.y }; }

	static void setIsExporting(bool bIsExporting);
	static void setWindowSize          (int width, int height);
	static void setAvailableAppViewSize(int width, int height);
	static void setExportSize          (int width, int height);

	static inline void setWindowTopLeft (int x, int y) { m_Window .setTopLeft(x, y); }
	static inline void setAvailableAppViewTopLeft(int x, int y) { m_AvailableAppView.setTopLeft(x, y); }

	static inline const glm::ivec2& getExportSize()           { return m_Export.size(); }
	static inline       glm::ivec2  getWindowTopLeft()        { return m_Window.topLeft(); }
	static inline const glm::ivec2& getWindowSize()           { return m_Window.size(); }
	static inline const glm::ivec2& getAvailableAppViewSize() { return m_AvailableAppView.size(); }

	static void ImGuiConstrainAppViewRatio();

	static inline void setRenderAreaResizedCallback(const std::function<void()>& callback) { OnRenderAreaResized = callback; }

private:
	static std::function<void()> OnRenderAreaResized; // Function pointer to be used as a callback

private:
	static RectSizePos m_Window;           // Size of the window, and it's position inside the screen
	static RectSizePos m_AvailableAppView; // Size of the available viewing area (excludes the docked ImGui windows), and its position inside the window
	static RectSize m_Export;              // Size of the exported images

	static bool m_bIsExporting; // Owned by Viewports because it needs to know it when deciding what the render size should be

	static bool m_bConstrainAppViewRatio;
	static float m_appViewConstrainedRatio;
};