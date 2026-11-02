# MotorNuestor

A custom 3D game engine and geometry visualizer built in **C++** and **OpenGL** for the Game Engines course (CITM/UPC). **MotorNuestor** features a clean separation between core engine logic and the editor interface, supporting 3D model loading via Assimp, texture importing with DevIL, a complete GameObject hierarchy, and intuitive camera controls.

* **GitHub Repository:** [abelnunez5/MotorNuestor](https://github.com/abelnunez5/MotorNuestor)

## Team Members & GitHub Profiles

* **Abel Núñez:** [abelnunez5](https://github.com/abelnunez5)
* **Pol Cuenca:** [Lexmaoo](https://github.com/Lexmaoo)
* **Andrea Vélez:** [veawenn](https://github.com/veawenn)

## How to Use

### Camera & Navigation Controls

* **WASD Movement:** Hold **Right Click + Any W A S D key** To move the camera like a first person shooter.
* **Speed Boost:** When WASD moving Hold **Shift** to move the camera faster.
* **Pan / Movement:** Hold **Alt + Left Click** and drag the mouse to move the camera.
* **Camera Rotation:** Hold **Right Click** and drag to freely rotate the camera view around its focal point.
* **Zoom:** Use the **Mouse Scroll Wheel** to zoom in and out.
* **Focus:** Press the **F** key to center and focus the camera on the selected geometry.

### Features & Operations

* **Model & Texture Drag & Drop:** 
  * Drag and drop any `.fbx` file directly into the scene window to load custom 3D geometry.
  * Drag and drop `.png` or `.dds` texture files to instantly apply them to the currently selected GameObject.
* **Editor Windows (View Menu):**
  * **Scene:** Renders the active 3D environment and models.
  * **Hierarchy:** Lists all loaded GameObjects; click an item to select it.
  * **Inspector:** Displays properties of the selected GameObject, including Transform components, Mesh details (with toggleable vertex and face normals), and Texture properties (with an option to view debug black-and-white checkerboard textures).
  * **Console:** Live logging system tracking Assimp geometry loading, DevIL texture importing, library initialization, application flow, and errors.
  * **Configuration:** Real-time FPS graphs, hardware detection, software version info (SDL3, OpenGL, DevIL), and module settings.
* **Toolbar Options:**
  * **File:** Exit the application.
  * **View:** Toggle editor panels on/off.
  * **3D Object:** Quick menu to spawn basic geometric primitives (Cube, Sphere, Plane, Cylinder).
  * **Help:** Direct links to GitHub documentation, bug reporting (issues), latest releases, and an **About** modal showing engine details, team members, used libraries, and MIT license.

## Extra Features

* *(No extra features outside of the mandatory requirements have been implemented for this milestone).*

## Additional Comments

* **Code Architecture:** The engine strictly decouples backend modules (`Engine/`) from the ImGui-based user interface (`Editor/`), ensuring clean separation of responsibilities.
* **Startup:** The application automatically initializes and loads the standard `baker_house.zip` model upon launch.

## License

This project is licensed under the terms of the **MIT License**. See the [LICENSE](LICENSE.md) file for details.
