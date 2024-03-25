<h3 align="center">OpenGL Pinball</h3>

  <p align="center">
    OpenGL Pinball is a game made using SDL2 and OpenGL's rendering pipeline. 
    <br />
    <a href="https://turtleteagames.com/projects/opengl_pinball">View Demo</a>
  </p>
</div>

## About The Project

![OpenGL_Pinball_Image_0](readme_assets/0.jpg)

Besides SDL2 and the use of GLText for the HUD, this was made completely from scratch with custom shaders using GLSL. These shaders were used to apply the following effects:
- Render textured models such as spheres, cylinders and cubes with calculated and changeable vertices
- Bling phong lighting model on all objects from a directional light and multiple point lights
- Texture mapping
- A skybox implemented with a OpenGL cubemap
- Environmental mapping reflecting the skybox
- Specular and diffuse texture maps
- Particle effects with billboarding towards the camera
- Bloom effect applied to objects that reach a certain threshold of brightness

The physics used were also made from scratch and were calculated every rendering cycle of the program. These physics calculations include:
- A 2D Quadtree used as a spatial data structure in order to keep track of stationary and moving objects and optimise collision calculations
- Bounding volumes for circles and axially aligned boxes
- 2D Collision detection for circle/circle, circle/box and box/box for detecting if a block is inside a Quadtree section.

![OpenGL_Pinball_Image_1](readme_assets/1.jpg)

## Built With

- C/C++ - https://isocpp.org/
- SDL2 - https://www.libsdl.org/
- Glad - https://glad.dav1d.de/
- GLText - https://github.com/go-gl/gltext

## Linked Libraries

Required libraries can be found in the `libs` folder, and should already be linked.

## How To Build

Open the `.sln` file in Visual Studio and build (Ctrl+Shift+B). 

## How To Run

You can run the application from source in Visual Studio using the Local Windows Debugger, or by downloading the release and running the `OpenGL_Pinball.exe` file.
