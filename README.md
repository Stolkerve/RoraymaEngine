
# RoraymaEngine
Is a 2D opengl game engine with editor and scripting.

- Use c++ 17.
- The scripting language is python (3.x).
- Have a text editor.
- Have a file explorer.
- Dynamic rendering

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
### Mac
`Why you use mac??????????????????`

## 
This project still in developing.
