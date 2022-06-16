# MinecraftOpenGL

This is a C++ Windows app that generates and renders a minecraft style voxel terrain using OpenGL.
I followed the OpenGL guide at [https://learnopengl.com](url) for the basics and added procedural mesh generation.

## How to open

Use Visual Studio 2019. Switch configuration to x86. Run. Hope it works.
The first couple lines of main.cpp contain some terrain generation settings you can change if you're bored.

## Documentation

### Initialization

#### 1. Set up window for OpenGL rendering

Boilerplate. GLFW creates the window and glad initiates communication with the graphics driver.

#### 2. Generate terrain

I stole perlin noise code from somewhere. It generates noise based on a seed.

The noise is used as a heightmap to generate the terrain mesh (Terrain.cpp).

1. Based on the highest and lowest point in the heightmap, determine the threshold for water and snow blocks.
2. Create the top of each block 'pillar' by reading from the heightmap
3. Add mesh strips that connect the tops of the pillars.

#### 3. Prepare for rendering

1. Load and compile shaders from files (ShaderProgram.cpp, Phong.vert, Phong.frag)
2. Specify texture coordinates of each block type
3. Load texture atlas (Texture.cpp, TextureAtlas.cpp)
4. Set shader uniforms (projection, texture, lighting)

### Loop

1. Calculate time since previous frame
2. Move and rotate camera based on input and delta time (Camera.cpp)
3. Update shader uniforms with new data from camera
4. Change light direction and clear color for day-night cycle
5. Draw terrain to frame buffer (Mesh.cpp)
6. Swap buffers to show result
