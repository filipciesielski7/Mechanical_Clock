<h1 align="center">
    Mechanical Clock 🕰
</h1>

_Also available in: [Polski](README.pl.md)_

## About

Mechanical clock 3D animation final project for the computer graphics &amp; visualization course at Poznan University of Technology

![animated gif of the clock](./preview.gif)

The main goal of this project was to implement 3D animation using [OpenGL](https://www.opengl.org//) software. In order to load models (mostly generated in [blender](https://www.blender.org/)) [assimp](https://github.com/assimp/assimp) library was used. Generated clock is showing current system time and clock pendulum as well as gears are working with proper speed. Additionaly, camera movement was also implemented.

## Controls

- &#8593;: moving forward
- &#8595;: moving backward
- Page up / P, Page down / L, &#8592;, &#8594;: looking around
- O: opening and closing the clock door

## Getting Started

This project works on both systems Linux and Windows.

### Prerequisites

To run this program on Linux, these libraries must be installed on your system:

- GLFW
- GLEW
- GLM
- Assimp

When it comes to Windows, to run this program [Visual Studio](https://visualstudio.microsoft.com/pl/) is needed.

### Linux

1. Clone the repo
   ```sh
   git clone https://github.com/filipciesielski7/Mechanical_Clock.git
   ```
2. Navigate into the _Linux_ directory
   ```
   cd Mechanical_Clock/Linux
   ```
3. Run this command in the Linux folder:
   ```
   make run
   ```

### Windows

The easiest way to run this animation on Windows system is to open **pl_szkielet_03_win.sln** file located in Windows directory using _Visual Studio_ and then run the **Local Windows Debugger** with **x86** option.
