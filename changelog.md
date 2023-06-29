# Changelog

- [🐣beta-6 WIP](#beta-6-wip)
- [🐣beta-5](#beta-5)
- [🐣beta-4](#beta-4)
- [🐣beta-3](#beta-3)
- [🐣beta-2](#beta-2)
- [🐣beta-1](#beta-1)
- [🐣beta-0](#beta-0)

## 🐣beta-6 WIP

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
