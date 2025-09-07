#ifndef WINDOW_HPP
#define WINDOW_HPP

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
        bool wireframeMode = false;
        bool faceCulling = true;

        Window
        (
            const char* title,
            int width,
            int height,
            int xPos,
            int yPos
        );

        bool Initialize();
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