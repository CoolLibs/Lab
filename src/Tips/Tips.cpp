#include "Tips.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"

namespace Lab {

auto all_tips() -> Cool::Tips
{
    static constexpr auto tips = std::array
    {
        "Our [Youtube tutorials](https://youtu.be/FK1sMPgcE4Q?list=PLuMMMaL5kN3mG6fSeSYTqC-sc_OTX3unQ) can help you get started with Coollab.",
            "In the \" " ICOMOON_TREE "Nodes\" window, right-click (or press A) to create your first node!", // NOLINT(bugprone-suspicious-missing-comma)
            "You can press ESCAPE to close these popups.",
#if defined(_WIN32)
            R"STR(If your image takes more than 2 seconds to render, Windows will automatically kill Coollab because it thinks it has frozen. You can increase this value to something like 60 seconds to make sure Coollab won't crash during an export at a high resolution, or when you artwork starts to become too heavy.
We **highly recommend** that you do change that, which is as simple as [following the steps in this great article](https://helpx.adobe.com/substance-3d-painter/technical-support/technical-issues/gpu-issues/gpu-drivers-crash-with-long-computations-tdr-crash.html).)STR",
#endif
            "You can change the color theme in the \" " ICOMOON_COG "Settings\" menu.",
            "Right-click on a node to replace it with a new node of the same category.",
            "Use F in the \" " ICOMOON_TREE "Nodes\" window to focus the view on the nodes.",
            "CTRL+click on a slider to input a value manually. (It also allows you to bypass the bounds of the slider!).",
            "In an angle or direction widget, hold SHIFT to snap to precise angles.",
            "Press ALT while scrolling in the \" " ICOMOON_IMAGE "View\" to zoom relative to the center of the view, instead of relative to the position of the mouse.",
            "You can rotate the 2D camera by holding SHIFT and scrolling the mouse wheel over the \" " ICOMOON_IMAGE "View\".",
            "Right-click on a widget to edit its metadata (range of a slider, etc.).",
            "You can save your own presets for a node if you like the parameters (and metadata) you found. You will find this option at the bottom of the the \" " ICOMOON_EQUALIZER "Inspector\" when a node is selected.",
            "The undo-history (CTRL+Z / CTRL+Y) is preserved even when you close and re-open Coollab! You can control its size in the \" " ICOMOON_COG "Settings\" menu.",
            R"(You can create your own color themes by going in the "Debug" menu (on the right of the menu bar) and selecting "Color Themes: Editor".)",

            // Bonus ImGui tips
            "Drag from the window menu button (the arrow button in the upper-right corner) to move all tabs at once.",
            "You can hold SHIFT to disable docking. Useful when you try to move a window around and docking gets in your way.",
            "Double-click on the lower-right corner of a floating window to fit its size to its content.",
            "Use TAB and SHIFT+TAB to cycle through the keyboard-editable fields of a window.",
            R"STR(Here are a few useful shortcuts:
  * While inputting text:
    * CTRL+Left/Right to word jump.
    * CTRL+A or double-click to select all. 
    * CTRL+X/C/V to use clipboard cut/copy/paste. 
    * CTRL+Z,CTRL+Y to undo/redo. 
    * ESCAPE to revert.
  * To navigate with the keyboard:
    * CTRL+TAB to select a window.
    * Arrow keys to navigate inside a window.
    * ENTER to activate a widget.
    * ESCAPE to deactivate a widget, close popup, exit child window.
    * ALT to jump to the menu of a window.
)STR",
    };

    return tips;
}

} // namespace Lab