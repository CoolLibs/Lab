# Changelog

- [🐣beta-18 WIP](#beta-18-wip)
- [🐣beta-17](#beta-17)
- [🐣beta-16](#beta-16)
- [🐣beta-15](#beta-15)
- [🐣beta-14](#beta-14)
- [🐣beta-13](#beta-13)
- [🐣beta-12](#beta-12)
- [🐣beta-11](#beta-11)
- [🐣beta-10](#beta-10)
- [🐣beta-9](#beta-9)
- [🐣beta-8](#beta-8)
- [🐣beta-7](#beta-7)
- [🐣beta-6](#beta-6)
- [🐣beta-5](#beta-5)
- [🐣beta-4](#beta-4)
- [🐣beta-3](#beta-3)
- [🐣beta-2](#beta-2)
- [🐣beta-1](#beta-1)
- [🐣beta-0](#beta-0)

## 🐣beta-18 WIP

- ✨ Every time input is now nicely formatted like "1h 27m 53s"
- ✨ The Output Window can now easily be turned fullscreen with <kbd>F10</kbd>
- ✨ When opening the Output Window, the aspect ratio automatically adapts to it
- ✨ The Output Window now doesn't have a title bar
- 🟦 Nodes: Added "MIDI Multi-Select with Transition" 
![](https://github.com/CoolLibs/Lab/assets/45451201/bb809bdf-c0ae-44c1-af53-2ef3961d99fa)
- 🐛 Allow access to your microphone on MacOS for Audio nodes

## 🐣beta-17

- ✨ Greatly improved video import: we now support videos with transparency, and GIFs
![](https://github.com/CoolLibs/Lab/assets/45451201/e5005eff-5b16-41ef-a4b1-74a46c481edc)
- ⚡ The performance of playing a video has been greatly improved
- 🐛 Video import now also works on Linux and MacOS
- 🐛 Many bug fixes around the video import and playback
- ✨ Improved MIDI support: buttons now have several modes (toggle, selector, pressed)
![](https://github.com/CoolLibs/Lab/assets/45451201/7826e34f-9210-4002-a145-385bf4107f29)
- 🟦 MIDI nodes: Added "Last MIDI button pressed", and you can now change the Min and Max output values of the "MIDI" node
- 🟦 Nodes: Added "Select" and "Multi-Select" which can typically be used in combination with MIDI buttons, to switch between various images / effects
![](https://github.com/CoolLibs/Lab/assets/45451201/c1692212-a6ea-4d2f-8e7d-4bc4f1c3f887)
- ✨ When exporting a video, if the output folder already contains frames from a previous export you now have 4 options, and by default we will prompt you to create a new folder
![](https://github.com/CoolLibs/Lab/assets/45451201/e5840829-21d5-4734-a388-5fd905dec17d)

## 🐣beta-16

- ✨ You can now import video files! Using the "Video from File" node.
![](https://github.com/CoolLibs/Lab/assets/45451201/fb7561df-f775-43d5-bbf4-1dedd32563bf)
- 🤏 The time in the timeline is now nicely formatted (as "3m 43s 512ms"). You can also input it like that (by CTRL+clicking on the timeline), as any combination of millisecond (ms) / second (s) / minute (m) / hour (h) / day (d) / week (w). For example "3m43".
- 🤏 The slider for some parameters (Zoom, Time Speed, etc.) now behaves logarithmically, meaning it will have equal precision in the 0-1 range as in 1-∞. Basically this means they are more practical to use.
- 🤏 When using a Drag widget, the mouse position now stays locked in place instead of wrapping around the screen.
- 🤏 Some node inputs now have a little info icon next to their pin, explaining what the parameter does in more details.
- 🤏 "Random" nodes now use an integer as a Seed, instead of any decimal number.
- 🤏 Improved Angle widget.
- 🤏 Improved final image size calculations based on desired aspect ratio.
- 🐛 Fixed image generation failure in some rare cases.
- 🐛 Fixed a crash when loading a project that was made in an older version of Coollab and that used a node that had been updated in Coollab.
- 🐛 Fix: Remove incompatible links when creating a link backward from a pin.

## 🐣beta-15

- ✨ Added a Time Speed on the timeline, which allows you to slow down or speed up your entire animation easily.
![](https://github.com/CoolLibs/Lab/assets/45451201/53cb9489-8924-4761-86bc-5191a00afa28)
- 🐛 Fixed camera movements when using particles
- 🐛 When dragging gizmos, mouse can now wrap around the screen
- 🐛 Fixed: When writing your own nodes, they were not detected.
- 🐛 Fixed a few nodes.
- 👩‍💻 The syntax for creating your own Coollab nodes is now 100% compatible with regular glsl syntax. You can now write structs, global variables, #define, etc.

## 🐣beta-14

- ✨ You can now create your own nodes! [Read the tutorial](https://coollab-art.com/Tutorials/Writing%20Nodes/Intro) to learn everything you need to know.
- 🐛 Fixed: Plugging a link into an input pin that already has a link now removes the old link. (This bug was introduced in the previous version).

## 🐣beta-13

- ✨ You can now copy-paste nodes! (CTRL+C / CTRL+V) You can even paste them from one project to another, or send them as text to a friend, who can then paste them in their instance of Coollab.
![](https://github.com/CoolLibs/Lab/assets/45451201/28a9c941-bc9a-49a6-b56f-5dadc2c005d7)
- ✨ Adding and removing nodes and links can now be undone and redone (CTRL+Z / CTRL+Y)
![](https://github.com/CoolLibs/Lab/assets/45451201/6e4d7e62-a919-4e71-81cf-76e5cb0b24e5)
- ✨ Changes to the 2D Camera can now be undone and redone (CTRL+Z / CTRL+Y)
- ✨ Applying a preset can now be undone and redone (CTRL+Z / CTRL+Y)

## 🐣beta-12

- ✨ Added OSC support! You can now control Coollab through OSC messages from your smartphone or another software on your computer! Just use the "OSC" node and select which channel's value you want to use.
![OSC](https://github.com/CoolLibs/Lab/assets/45451201/9e0b1284-97fd-4c4e-8ba5-0909ada8b242)
- ⚡ Improved performance when using Midi input.
- 🐛 Fixed a rare crash when using Midi input.
- 🐛 Fixed a bug that caused the Nodes window to zoom out a bit each time you opened Coollab.

## 🐣beta-11

- ✨ Added particles! This a very promising prototype that you can already use today. An important overhaul of the system will come at some point in a (probably distant) future. (NB: Unfortunately particles are not currently available on MacOS, and will not be for a very long while).
![Particles](https://github.com/CoolLibs/Lab/assets/45451201/7a8da2ef-df37-44fe-bf93-6cd37638c9f6)
- ✨ Improved the randomness of our Random and Noise nodes.
- ✨ Prevented mouse from getting blocked on the screen edges while dragging a widget or the camera. Instead the mouse wraps around.
- ✨ Prevented mouse from getting blocked on the screen edges while dragging a node or a link or a selection rectangle in the nodes view. Instead the canvas starts translating as expected.
- ✨ Greatly improved quite a few nodes, most notably "Glow".
- ✨ Added quite a few nodes, most notably "Blur" and "Adaptive Halftone".
- 🤏 You can now pan the Nodes view with the middle mouse button (and you can still use the right button as you used to).
- ✨ Added A4/A3/A2/A1 aspect ratio (when selecting the size of the View and the exported images).
- 🐛 Fixed crash on Windows when an image export took longer than 2 seconds.
- 🐛 Fixed freeze on Linux when exporting a video.
- 🐛 Fixed: we won't request use of your microphone unless we actually need it (e.g. as soon as you start using it as an audio input in Coollab).

## 🐣beta-10

- ✨ Added Audio support! You can now import an audio file and play it while you generate your images. You can also use its volume and waveform to control your images. You can also receive sound from an input device (microphone, etc.). Check out [our audio tutorial](https://coollab-art.com/Tutorials/Features/Audio) to learn everything about it!
![node_spectrum](https://github.com/CoolLibs/Lab/assets/45451201/17e77692-7941-4983-b2cf-a30b13aeee12)
- 🟦 Added new nodes that you can use to display the audio features: "Fill Function", "Function to Shape" and "Add Displacement".
- 💄 Added icons in the Commands menu.
![open_audio_config_command](https://github.com/CoolLibs/Lab/assets/45451201/3a49dd4b-1247-4329-8ed0-81004940ae77)
- 🐛 Fixed crash on Linux when opening a file explorer.
- 🐛 Temporary workaround: on Linux the history isn't saved when closing Coollab, in order to avoid a crash.
- 🚚 On Linux, moved user-data folder to *.local/share*

## 🐣beta-9

- ✨ Added MIDI support! You can now plug-in your MIDI keyboard, select your knob / slider by index, and use its value to control parameters of your nodes.
![image](https://github.com/CoolLibs/.github/assets/45451201/5a8d4950-57a0-4282-b549-6c66487448c3)
- ✨ Added the "Paint" blend mode.

| ![image](https://github.com/CoolLibs/.github/assets/45451201/6f57a43d-a422-4056-81e1-c691e4c85d84)  | ![image](https://github.com/CoolLibs/.github/assets/45451201/9b12b5bf-7f15-408b-93e9-552e0caa30ea)  |
|---|----|
| "Light" mode (what was already there in Coollab) | "Paint" mode (the new blend mode) |

- ✨ Added an Output window that you can project during live shows, while still having your View window on your screen to move the camera and the widgets. To open this window, go in the `Commands` menu and select `Open output window`.
![image](https://github.com/CoolLibs/.github/assets/45451201/3d6a14ec-69ca-44b1-81e0-1d4139b72544)
- ✨ Added "Open Backup" in case you accidentally refused to save your unsaved changes.
![image](https://github.com/CoolLibs/.github/assets/45451201/99287d7b-15a1-480b-98d4-0ca5c4777b86)
- 🐛 Fix: the transparency information was sometimes getting lost between nodes.
- 🐛 Fix: crash on Linux "Too many open files".
- ⚡️ Fix lag when editing the color gradient on a Color Ramp node.
- 👩‍💻 Replaced CIELAB color space with Oklab
- 👩‍💻 Replaced HSLuv color space with Okhsl

## 🐣beta-8

- ✨ Added project files: you can now save and open projects, allowing you to keep and share your work!
![image](https://github.com/CoolLibs/Lab/assets/45451201/00270343-3a45-4e92-93cc-729f0f674c1e)
- ✨ Coollab now has an installer! You don't need to download the raw executable anymore.
![image](https://github.com/CoolLibs/Lab/assets/45451201/a1042659-e003-4dcf-b917-79505c84c28e)
- ✨ Added the "Webcam" node!
![image](https://github.com/CoolLibs/Lab/assets/45451201/77fd1a74-2e7c-43eb-914a-90a711ce2cae)
- ✨ Added feedback loops! Check out the "Feedback" node.
![ezgif-5-a3c56b1c92](https://github.com/CoolLibs/Lab/assets/45451201/d6513535-6f53-4932-b260-20a54c032380)
- 🚚 Renamed "Space Transformation" category as "2D Modifier".
- 🐛 Fix: some nodes had the wrong color.
- 🐛 Fix: on Linux, for some window managers like i3, the context menu was not behaving properly. (Now by default we disable multi-viewport in you use one of these custom window managers, which fixes the issue but prevents you from dragging windows outside of the main Coollab window. This can be changed in the Settings menu.) 

## 🐣beta-7

- ✨ Only one camera (either 2D or 3D) can be active at the same time.
- ✨ Improved "Distortion Map" node.
- ✨ Added several "3D Shape from 2D" nodes.
- 🚚 Renamed "Boolean" categories as "Blend".
- 🚚 Renamed "Blend" nodes as "Mix".
- 🚚 Moved "Time" node to the "Input" category.
- 🐛 Fixed implicit conversion between numbers and angles: a number equal to 1 (white) now corresponds to a full turn (360 degrees).

## 🐣beta-6

- ✨ Added many Shape Booleans.
- ⚡️ Big performance improvements. If you experienced lag spikes before, they should be gone now!
- 🐛 Fix: some nodes had the wrong color.

## 🐣beta-5

- ✨ Added gizmos on the view that allow you to edit Point2Ds visually.
![Animation](https://github.com/CoolLibs/Lab/assets/45451201/b2a5ad91-e9bf-42d6-ab7a-c60e86e97c40)

- ✨ Added Bezier Curve node.
- ✨ Added a full 3D renderer node.
![Animation-min](https://github.com/CoolLibs/Lab/assets/45451201/5996fb72-258b-46ff-b87d-4195bda21215)

- ✨ Added tips that will show up from time to time and teach you about the subtleties and shortcuts of Coollab.
![image](https://github.com/CoolLibs/Lab/assets/45451201/112c4431-5e68-4617-9bcb-6591755aae05)

- ✨ Main input pins now have a different icon to distinguish them from the other pins.
![image](https://github.com/CoolLibs/Lab/assets/45451201/73191129-3c1a-4fb0-afa8-6651100399aa)

- 🐛 Fix: prevent panning the nodes while you are panning the camera.
- 🐛 Fixed the camera controls not working when the View window was on another screen.

## 🐣beta-4

- ✨ Improved quite a few existing nodes.
- ✨ Added new nodes. Check out our "Cloud" renderer for 3D Shapes!
- ✨ Node pins now have a color that reflects the kind of node that you can plug into them. (e.g. a pin that wants a Shape 3D will be yellow, just like the Shape 3D nodes.)
- ✨ Smarter automatic main node selection.
- ✨ All nodes can now be viewed by themselves, without requiring a "renderer" node (which used to be the case for Curves and 3D Shapes).
- ✨ Added buttons on the view to freeze / enable the 2D and 3D cameras.
- 🐛 Fixed some effects (Space Transformations) not being applied on top of 3D shapes.

## 🐣beta-3

- ✨ Added many 3D nodes.
- ✨ Added a Frame (Comment) node.
- ✨ In the "Export" menu, added a button to share your image online in [Coollab's Gallery](https://coollab-art.com/Gallery).
- 💄 Completely overhauled UI.
- 💄 New logo.
- 💄 Added Settings to change your color theme.
- 💄 Improved the Dark color theme.
- 💄 Added a Light color theme.
- 💄 Added an option to use the color theme set by your OS (Dark or Light).
- 💄 The View now uses a fixed aspect ratio by default (you can change this in the Preview menu).
- 💄 Improved Cameras window + added an option to lock one of the two cameras (2D or 3D) when using both 2D and 3D nodes.
- 🐛 Fixed the camera controls when using a fixed aspect ratio or fixed number of pixels.
- 🐛 Fix: the nodes categories were not sorted on MacOS.
- 🐛 Fix: could not place a node that had the same name as its category.
- 👩‍💻 Node files: now support the `#define` macro, just like any glsl file.
- 👩‍💻 Node files: added boolean and matrices types for INPUTs, main function and helper functions (they can be used anywhere like any other type now).

## 🐣beta-2

- Fix "Reserved built-in name" error on some GPUs
- Fix: properly display error messages when loading (parsing) a node failed (very useful for people creating new nodes)
- Started using icons all over the place (window titles, buttons, menus, ...)
- Many more nodes, and improvements to existing nodes

## 🐣beta-1

- Fix the huge visual glitches that appeared on some computers
- Image Node: fix the ghost error messages that it would create and that couldn't be removed
- Nodes:
  - New nodes
  - Fixes
  - Improvements
  - Added presets for some of them
  - Reorganized categories a bit
- All shapes are now nicely anti-aliased
- Added the debug options menu to the released executables
- Added debug option to get an info dump in order to help developers debug

## 🐣beta-0

- First version
