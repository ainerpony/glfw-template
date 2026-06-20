## About
- GLFW+OpenGL canvas template for personal use.
## Build
### With CMake
  - Install cmake
  - For Linux, run: `./build.sh`
  - For Windows, run: `./build.ps1`
  - Default output dir: `build_xxx/Release/`
    - You can specify the output dir by: `-D EXEC_OUTPUT_DIR="../src"`
      - Example: `$ cmake -B build -D EXEC_OUTPUT_DIR="../src"`
    - and then: `$ cmake --build build`
### Manually
- Install C++ complier, such as g++, clang++, etc.
- You will need to figure out how to get GLFW lib, build from the source code or download the [pre-compiled binaries](https://www.glfw.org/download).
- For GNU:
  - `$ g++ -o main ./src/main.cc ./thirdparty/glad/glad.c  -I"./thirdparty" -I"./thirdparty/glfw-3.4/include" -L"<path to libglfw3.a>" -lglfw3`