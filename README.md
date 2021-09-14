
# RoraymaEngine
Is a 2D opengl game engine with editor and scripting.
 
![alt text](https://github.com/Stolkerve/RoraymaEngine/blob/main/assets/Editor.PNG?raw=true)

## Features
- C++ 17.
- Python (3.x) as scripting language.
- Dynamic rendering
- Text rendering
- Simple
- No bloat

## Features not implemented
- Audio system
- Build system
- Support for Linux, Android
- 2D physics
- Lighting

## Dependencies
- Premake5
- GLFW
- ImGui
- stb_image
- pybind_11
- ImGuiColorTextEdit
- glm
- yaml-cpp
- spdlog
- Glad
- Portable_File_Dialogs

## Compilation
First, you need download premake5 and drop it in the project foldel and type in the console.

### windows
`./premake5.exe vs2019`
This will generate a vs solution file and all the projects.
### Linux
First, delete a line of code in Engine/src/RoraymaEngine/Plataform.hh

```cpp
#elif __linux__
this -----> #error blablabla
```

`./premake5 gmake2`
This will generate MakeFile. And for compile type.
`make`
Once it finish compiling, a folder named bin will be create with the binary, then you must drop from the Editor folder imgui.ini, keys_map.yaml, window_config.yaml, assets to where is the binary.
### Mac
`Why you use mac??????????????????`

## 
This project still in developing.
