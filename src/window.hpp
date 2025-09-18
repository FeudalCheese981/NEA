#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "orbitLine.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include "grid.hpp"
#include "satellite.hpp"

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

struct LaunchUI
{
    bool isOpen = true;
    float launchSiteLatitude_Degrees = 0.0f;
    float launchSiteLongitude_Degrees = 0.0f;
    float launchAzimuth_Degrees = 0.0f;
    float MecoAltitude_km = 100.0f;
    float MecoVelocity_ms = 7900.0f;
    float MecoDownrangeDistance_km = 0.0f;
    float MecoFlightPathAngle_Degrees = 0.0f;
};

struct objectInfo
{

};

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

        int anitAiliasingSamples = DEFAULT_MSAA_SAMPLES;
        int segmentCount = DEFAULT_SEGMENT_COUNT;

        float FOV = DEFAULT_FOV;
        float nearPlaneDist = 0.1f;
        float farPlaneDist = DEFAULT_RENDER_DISTANCE;
    
        double simRate = DEFAULT_SIM_RATE;
        double simTime = 0.0;
        double deltaTime = DEFAULT_DELTA_T;
        double frameTime = 0.0;
        double runTime = 0.0;

        bool fullscreen = false;
        bool multisample = true;
        bool wireframeMode = false;
        bool faceCulling = true;
        bool displayFPS = false;
        bool displaySimInfo = false;
        bool displayControls = true;
        bool displayLaunch = false;
        bool paused = false;

        std::unique_ptr<Shader> lightShader;
        std::unique_ptr<Shader> shaderProgram;
        std::unique_ptr<Shader> lineShader;

        std::unique_ptr<Light> sun;
        std::unique_ptr<Sphere> planet; 
        std::unique_ptr<OrbitLine> orbit1;

        std::unique_ptr<Satellite> sat1;
        std::unique_ptr<Sphere> sat1Object;

        std::vector<double> fpsTrack;
        double currentFPS = 0.0;
        double averageFPS = 0.0;
        double minFps = -1.0;

        std::vector<LaunchUI> launchUiWindows;

        ImGuiIO* io = nullptr;

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

        void IncreaseSimRate();
        void DecreaseSimRate();
        void ResetSimRate();

        void Pause();
        void Resume();

        void RenderLoop();

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
};

#endif