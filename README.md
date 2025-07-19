# Anguis

A 3D multiplayer third and first person snake game <br>
Multiplayer *may* have some bugs :) <br>
All models/textures by me. Music is from opengameart.org. SFX were created through SFXR <br>

![screenshot](screenshot.png)
<br>

### Controls
Mouse to move
TAB to change POV (toggles between first and third person)
Left Click to move faster

### Building
```
git clone https://github.com/STCollier/anguis
cd anguis
mkdir build
cd build
cmake ..
make
cd ../ && ./build/anguis
```

### Details
- Written in C++26 and OpenGL 4.1
- Dependencies
    - GLAD (OpenGL loader)
    - GLFW (windowing & input)
    - GLM  (vector math)
    - ImGui (GUI)
    - socket.io (networking)
    - stb_image (loading images)
    - dr_wav (loading wav files for OpenAL)
    - openal-soft (Open Audio Library for playing audio)
- Graphical Features
    - .obj loader
    - HDR & Bloom
    - Real-time shadows
    - Multiplayer
    - Particle system
    - Audio
    - GUI done purely with ImGui <3