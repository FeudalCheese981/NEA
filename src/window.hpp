#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "orbit.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include "grid.hpp"

const unsigned int OPENGL_VERSION_MAJOR = 4; // OpenGL version 4.6 core
const unsigned int OPENGL_VERSION_MINOR = 6;
const unsigned int OPENGL_PROFILE = GLFW_OPENGL_CORE_PROFILE;
const int DEFAULT_MSAA_SAMPLES = 8; // MSAA samples
const int DEFAULT_SEGMENT_COUNT = 1024; // How smooth spheres/curves are drawn 
const float DEFAULT_FOV = 45.0f;
const float DEFAULT_RENDER_DISTANCE = 1000000.0f;
const double DEFAULT_SIM_RATE = 1.0;
const double DEFAULT_DELTA_T = 1.0 / 1000.0;

enum ObjectType { SPHERE, LIGHT, ORBIT, GRID };

class Window
{
    public:
        const char* windowTitle;

        int windowXPos;
        int windowYPos;
        int windowWidth;
        int windowHeight;

        Camera camera;
        GLFWwindow* window;

        std::vector<const char*> saveNames;
        std::vector<Object> objects;
        std::vector<Shader> shaders;

        int MSAAsamples = DEFAULT_MSAA_SAMPLES;
        int segmentCount = DEFAULT_SEGMENT_COUNT;

        float FOV = DEFAULT_FOV;
        float nearPlaneDist = 0.1f;
        float farPlaneDist = DEFAULT_RENDER_DISTANCE;
    
        double simRate = DEFAULT_SIM_RATE;
        double simTime = 0.0;
        double deltaTime = DEFAULT_SIM_RATE;

        bool fullscreen = false;
        bool multisample = true;
        bool wireframeMode = false;
        bool faceCulling = true;
        bool displayFPS = false;

        std::unique_ptr<Shader> lightShader;
        std::unique_ptr<Shader> shaderProgram;

        std::unique_ptr<Light> sun;
        std::unique_ptr<Sphere> planet; 

        /*
        // shader
        Shader lineShader = Shader("shaders\\line.vert", "shaders\\line.frag");
        Shader lightShader = Shader("shaders\\light.vert", "shaders\\light.frag");
        Shader objectShader = Shader("shaders\\object.vert", "shaders\\object.frag");

        // planet
        Sphere planet(1.0f, SEGMENT_COUNT, SEGMENT_COUNT / 2, glm::vec4(0.5f, 0.75f, 0.75f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), SPHERE_COLOR_RGB);
        planet.Place();
        
        Sphere moon(0.27f, SEGMENT_COUNT, SEGMENT_COUNT / 2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -60.2696f, 0.0f), SPHERE_COLOR_DEFAULT);
        moon.Place();

        // orbit
        Orbit orbitLine
        (
            SEGMENT_COUNT,
            2.95035f,
            0.6504292532066034f,
            glm::radians(63.940270121756825f),
            glm::radians(32.1116423594561f),
            glm::radians(-96.06445668472178f),
            0.0f,
            glm::vec4(0.36f, 0.22f, 0.82f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        );
        orbitLine.Place();
        
        // grid
        Grid gridDark(1000.0f, 100, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        gridDark.Place();

        Grid gridBright(1000.0f, 10, glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        gridBright.Place();

        Grid gridFull(1000.0f, 2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        gridFull.Place();

        // light
        glm::vec3 sunPos = glm::vec3(0.0f, -23544.2f, 0.0f);
        // sunPos = glm::rotate(sunPos, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        Light sun(109.3f, 32, 16, glm::vec4(253.0f / 255.0f, 251.0f / 255.0f, 211.0f / 255.0f, 1.0f), sunPos);
        sun.Place();
        sun.SendLightInfoToShader(objectShader);
        */

        Window
        (
            const char* title,
            int width,
            int height,
            int xPos,
            int yPos
        );

        void Initialize();
        void Terminate();

        void ResizeWindow(int width, int height);
        void KeyInput(int key, int scancode, int action, int mods);
        void ScrollInput(double xOffset, double yOffset);

        void OpenSaveFiles();
        void LoadSaveFile(const char* filename);
        void SaveSaveFile();

        void AddObject(ObjectType type);
        void UpdateObjects();
        void DrawObjects();
        void DeleteObject();

        void AddShader(const char* vertexFile, const char* fragmentFile);
        void DeleteShader();

        void DrawUI();

        void EnableWireframeMode();
        void DisableWireframeMode();

        void EnableFaceCulling();
        void DisableFaceCulling();

        void RenderLoop();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
