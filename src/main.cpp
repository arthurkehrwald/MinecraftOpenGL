#define TERRAIN_AMPLITUDE 30.0f
#define TERRAIN_SIZE_X 64
#define TERRAIN_SIZE_Z 64
#define WIN_WIDTH 1280
#define WIN_HEIGHT 800


#pragma once
// Local Headers
#include "ShaderProgram.h"
#include "TextureAtlas.h"
#include "Mesh.h"
#include "Camera.h"
#include "Terrain.h"
#include "PerlinNoise.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

const glm::vec3 cameraPos(0.0f, TERRAIN_AMPLITUDE, 0.0f);
Camera camera(cameraPos, 45.0f, -25.0f);
glm::vec2 prevCursorPos(400.0, 300.0);
bool hasCursorMoved = false;

void mouseCallback(GLFWwindow* window, double xPos, double yPos);

int main(int argc, char* argv[])
{
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr)
    {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // Configure Window    
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouseCallback);

    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::vector<std::vector<int>> heightMap(TERRAIN_SIZE_X, std::vector<int>(TERRAIN_SIZE_Z, -1));

    const int noiseSeed = 120;
    PerlinNoise perlinNoise(noiseSeed);

    for (int x = 0; x < TERRAIN_SIZE_X; ++x)
    {
        for (int z = 0; z < TERRAIN_SIZE_Z; ++z)
        {
            double sampleCoordX = (double)x;
            double sampleCoordZ = (double)z;
            double noise = perlinNoise.noise(0.05 * sampleCoordX, 0.8, 0.05 * sampleCoordZ);
            heightMap[x][z] = (int)(noise * TERRAIN_AMPLITUDE);
        }
    }

    ShaderProgram shaderProgram("./Assets/Shaders/Phong.vert",
        "./Assets/Shaders/Phong.frag");
    shaderProgram.activate();

    std::map<TextureAtlas::SubTexture, glm::vec4> subTextureCoords;
    subTextureCoords[TextureAtlas::SubTexture::dirtTop] = glm::vec4(0.0f, 0.5f, 0.25f, 0.75f);
    subTextureCoords[TextureAtlas::SubTexture::dirtSideTop] = glm::vec4(0.0f, 0.25f, 0.25f, 0.5f);
    subTextureCoords[TextureAtlas::SubTexture::dirt] = glm::vec4(0.0f, 0.0f, 0.25f, 0.25f);
    subTextureCoords[TextureAtlas::SubTexture::snowTop] = glm::vec4(0.25f, 0.5f, 0.5f, 0.75f);
    subTextureCoords[TextureAtlas::SubTexture::snowSideTop] = glm::vec4(0.25f, 0.25f, 0.5f, 0.5f);
    subTextureCoords[TextureAtlas::SubTexture::sand] = glm::vec4(0.25f, 0.0f, 0.5f, 0.25f);
    subTextureCoords[TextureAtlas::SubTexture::water] = glm::vec4(0.5f, 0.0f, 0.75f, 0.25f);
    TextureAtlas textureAtlas("./Assets/Textures/texture_atlas.png", GL_RGBA, subTextureCoords);
    textureAtlas.activate(0);
    shaderProgram.setUniform("DiffuseTexture", 0);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    shaderProgram.setUniform("LightCol", lightColor);

    glm::vec3 lightDirection(-1.0f, -2.0f, -1.0f);
    shaderProgram.setUniform("LightDir", lightDirection);

    float ambientIntensity = 0.4f;
    shaderProgram.setUniform("AmbientIntensity", ambientIntensity);

    Terrain terrain(heightMap, textureAtlas);
    terrain.activate();

    glm::mat4 model = glm::mat4(1.0f);
    shaderProgram.setUniform("Model", model);

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    shaderProgram.setUniform("View", view);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    shaderProgram.setUniform("Projection", projection);

    float timeAtPrevFrame = glfwGetTime();

    // Rendering loop 
    while (!glfwWindowShouldClose(mWindow))
    {
        float deltaTime = glfwGetTime() - timeAtPrevFrame;
        timeAtPrevFrame = glfwGetTime();

        // Process input
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        camera.processMoveInput(mWindow, deltaTime);

        // Set shader uniforms        
        view = camera.getView();
        shaderProgram.setUniform("View", view);
        shaderProgram.setUniform("Model", model);
        shaderProgram.setUniform("WorldCameraPos", camera.getPos());
        float t = sin(glfwGetTime() * 0.2f);
        glm::vec3 lightDirection(t, t * -2.0f, 1.0f);
        shaderProgram.setUniform("LightDir", lightDirection);
        const glm::vec3 dayLightColor(0.95f, 0.95f, 0.8f);
        const glm::vec3 daySkyColor(0.5f, 0.75, 0.95f);
        const glm::vec3 dawnLightColor(0.8f, 0.6f, 0.4f);
        const glm::vec3 dawnSkyColor(0.8f, 0.6f, 0.8f);
        const glm::vec3 nightLightColor(0.2f, 0.1f, 0.4f);
        const glm::vec3 nightSkyColor(0.05f, 0.05f, 0.1f);
        glm::vec3 skyColor;
        if (t > 0.0f)
        {
            lightColor = dayLightColor * t + dawnLightColor * (1.0f - t);
            skyColor = daySkyColor * t + dawnSkyColor * (1.0f - t);
        }
        else
        {
            lightColor = dawnLightColor * (1.0f - abs(t)) + nightLightColor * (abs(t));
            skyColor = dawnSkyColor * (1.0f - abs(t)) + nightSkyColor * (abs(t));
        }
        shaderProgram.setUniform("LightCol", lightColor);

        // Background fill color
        glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        terrain.draw();

        // Flip buffers and draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (!hasCursorMoved)
    {
        prevCursorPos = glm::vec2(xPos, yPos);
        hasCursorMoved = true;
    }
    const float cursorSensitivity = 0.1f;
    glm::vec2 cursorOffset(xPos - prevCursorPos.x, prevCursorPos.y - yPos);
    cursorOffset *= cursorSensitivity;
    camera.processLookInput(cursorOffset);
    prevCursorPos = glm::vec2(xPos, yPos);
}