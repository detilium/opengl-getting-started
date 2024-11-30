# OpenGL Getting started

This project is built by following the guide at https://learnopengl.com, and acts as a guide to understanding how OpenGL works.

## Prerequisites
This project specifically targets OpenGL 3.3 and uses GLAD and GLFW. The complete guide to download and install dependencies, can be found at https://learnopengl.com/Getting-started/Creating-a-window.

To get linking and includes to work out of the box, the following paths are expected to be used:
- `D:/Development/Game dev - Workspace/OpenGL/Lib`
- `D:/Development/Game dev - Workspace/OpenGL/Include`

If you prefer a different location, you'd have to setup include directories in the project yourself.

### Adding GLFW
GLFW has been compiled ourselves from their source code.
1. Download the source code at https://www.glfw.org/download.html
2. Download and install CMake from https://cmake.org/download/
3. Build GLFW with CMake
4. Open GLFW.sln
5. Build the solution
6. Copy glfw3.lib to the lib path
7. From the GLFW source code, copy the `/include` folder to include path.

### Adding GLAD
GLAD is manually downloaded from their web service at https://glad.dav1d.de/.
> _⚠ WARNING: Ensure that you're using GLAD1 web service at the provided link. A GLAD2 also exists, but won't compile is this scenario._
1. Navigate to the website
2. Choose OpenGL version 3.3
3. Choose `Core` as `Profile`
3. Make sure `Generate a loader` is checked
4. Press `Generate`
5. Once downloaded, copy the contents of the `/include` folder to include path (_both the `glad` and `KHR` folders_)
6. Copy the file `glad.c` to the project