# SpaceBoxes

SpaceBoxes is a very creative name for a small game made with C++ and OpenGL,
applying learning and concepts from the Getting Started section of https://learnopengl.com/.

![spaceboxes-gif](https://github.com/leoalopes/space-boxes/blob/main/assets/spaceboxes.gif?raw=true)

## Building

The project is configured to be built with CMake. You can build everything at once by running:

```
cmake -S . -B build
cmake --build build
```

To run the executable, you can do:

```
./build/app/shooter
```

## Structure

The project structure is divided into `core`, `app` and `dependencies`. Those are built with separate CMakeLists.txt files.

The `dependencies` folder includes GLAD, GLM and STB.
Another dependency is GLFW3, that is required to be installed in the system and is not built by this project.

The `core` folder includes all code written for the project as an scaffolding for the application code.
Here we have classes for the shaders, textures, colliders, buffers, etc.

Finally, the `app` folder has the main file and any other files that use the `core` library to create the game.
