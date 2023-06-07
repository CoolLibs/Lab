#include "Tips.h"

namespace Lab {

auto all_tips() -> Cool::Tips
{
    static constexpr auto tips = std::array{
        "In the Nodes window, right-click or press A to create your first node!",
        "You can change the color theme in the Settings, or make it match your OS's color theme.",
        "Right-click on a node to replace it with a new node of the same category.",
        "Use F to focus on the nodes",
        "You can hold SHIFT to disable docking. Useful when you try to move a window around and docking gets in your way.",
        "The undo-history (CTRL+Z) of your modifications is preserved even when you close and re-open CoolLab! You can control its size in 'Settings > History Size'",
        "Talk about TDR to avoid crashes (https://www.artstation.com/blogs/sebastianbracht/ovQg/workaround-for-the-windows-tdr-crash-issue-while-using-substance-painter, https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiV6LmO27_9AhXmcaQEHYw5AdUQFnoECBMQAQ&url=https%3A%2F%2Fsubstance3d.adobe.com%2Fdocumentation%2Fspdoc%2Fgpu-drivers-crash-with-long-computations-tdr-crash-128745489.html&usg=AOvVaw3NUHI7sPGmV__7gxQR4w4U)",
        "Drag from window menu button (upper-left button) to undock an entire node (all windows).",
        "Ctrl+click on a slider to input a value manually (and it also allows you to bypass the bounds of the slider !)",
        "Double-click on title bar to collapse window.",
        "Double-click on lower right corner to auto fit window to its contents.",
        "TAB/SHIFT+TAB to cycle through keyboard editable fields.",
        "Text editor: \n Hold SHIFT or use mouse to select text.\nCTRL+Left/Right to word jump.\nCTRL+Shift+Left/Right to select words.\nCTRL+A or Double-Click to select all.\nCTRL+X,CTRL+C,CTRL+V to use OS clipboard/\nCTRL+Z,CTRL+Y to undo/redo.",
        "ESCAPE to revert text to its original value.",
        "Double-click on title bar to collapse window.",
        "CTRL+Tab to select a window. ImGui::BulletText('While inputing text:\n'); ImGui::Indent(); ImGui::BulletText('CTRL+Left/Right to word jump.'); ImGui::BulletText('CTRL+A or double-click to select all.'); ImGui::BulletText('CTRL+X/C/V to use clipboard cut/copy/paste.'); ImGui::BulletText('CTRL+Z,CTRL+Y to undo/redo.'); ImGui::BulletText('ESCAPE to revert.'); ImGui::Unindent(); ImGui::BulletText('With keyboard navigation enabled:'); ImGui::Indent(); ImGui::BulletText('Arrow keys to navigate.'); ImGui::BulletText('Space or enter to activate a widget.'); ImGui::BulletText('Return to input text into a widget.'); ImGui::BulletText('Escape to deactivate a widget, close popup, exit child window.'); ImGui::BulletText('Alt to jump to the menu layer of a window.');",
        "In an angle or direction widget, hold SHIFT to snap to precise angles.",
        "You can rotate the 2D camera by holding SHIFT and scrolling the mouse wheel over the View.",
        "Use ALT to zoom relative to the center of the view instead of relative to the position of the mouse.",
        "Right-click on a widget to edit its metadata (range of a slider, etc.)",
        "You can save your own presets on each nodes if you like the parameters you found.",
    };

    return tips;
}

} // namespace Lab