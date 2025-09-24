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
const float DEFAULT_RENDER_DISTANCE = 1.0e12f;
const double DEFAULT_SIM_RATE = 1.0;
const double DEFAULT_DELTA_T = 1.0 / 1000.0;

// planetary Info
const float EARTH_MASS = 5.97e24;
const float EARTH_RADIUS = 6.37e6;

enum ObjectType { SPHERE, LIGHT, ORBIT, GRID };

struct LaunchUI
{
    bool isOpen = true;
    char name[30];
    float mass_kg = 10.0f;
    float radius = 0.01f;
    float latitude_Degrees = 0.0f;
    float longitude_Degrees = 0.0f;
    float azimuth_Degrees = 90.0f;
    float altitude_km = 100.0f;
    float velocity_ms = 7900.0f;
    float downrangeDistance_km = 0.0f;
    float flightPathAngle_Degrees = 0.0f;
    float color[3] = {1.0f, 1.0f, 1.0f};
};

class Window
{
    public:
        const char* windowTitle;

        // window pointer
        GLFWwindow* window;
    
        // Camera
        Camera camera;

        // Shaders
        std::unique_ptr<Shader> lightShader;
        std::unique_ptr<Shader> objectShader;
        std::unique_ptr<Shader> lineShader;

        // store window info
        int windowXPos;
        int windowYPos;
        int windowWidth;
        int windowHeight;

        // window settings
        int antiAiliasingSamples = DEFAULT_MSAA_SAMPLES;
        int segmentCount = DEFAULT_SEGMENT_COUNT;
        float FOV = DEFAULT_FOV;
        float nearPlaneDist = 0.1f;
        float farPlaneDist = DEFAULT_RENDER_DISTANCE;
        
        // boolean variables
        bool fullscreen = false;
        bool multisample = true;
        bool wireframeMode = false;
        bool faceCulling = true;
        bool displayFPS = false;
        bool displaySimInfo = false;
        bool displayControls = true;
        bool displaySatellites = true;
        bool paused = false;

        // physics variables
        double accumulator;
        double deltaTime = DEFAULT_DELTA_T;

        // sim variables
        double runTime = 0.0;
        double simTime = 0.0;
        double simRate = DEFAULT_SIM_RATE;

        // fps tracker variables
        double fpsPrevDisplayTime;
        double fpsCrntDisplayTime;
        double frameTime = 0.0;
        double currentFPS = 0.0;
        double averageFPS = 0.0;
        double minFps = -1.0;
        std::vector<double> fpsTrack;

        // objects for lazy initialization
        std::unique_ptr<Light> sun;
        std::unique_ptr<Sphere> planet; 
        std::unique_ptr<OrbitLine> orbit1;

        // vector containing ui windows for launching satellites
        std::vector<LaunchUI> launchUiWindows;

        // satellites
        std::vector<Satellite> satellites;

        // imgui IO pointer
        ImGuiIO* io = nullptr;

        // constructor
        Window(const char* title, int width, int height, int xPos, int yPos);

        // Window Setup / Shutdown
        void Initialize();
        void Terminate();

        // GLFW Callbacks
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

        // Input Management
        void ResizeWindow(int width, int height);
        void KeyInput(int key, int scancode, int action, int mods);
        void ScrollInput(double xOffset, double yOffset);

        // Object Management
        void AddObject();
        void UpdateObjects();
        void DeleteObject();

        // Satellites
        void AddSatellites(const char* name, float mass, float longitude, float latitude, float azimuth, float altitude, float velocity, float distance, float FPA, float radius, float color[3]);
        void UpdateSatellites();
        void DeleteSatellites(const char* name);
        void TerminateDeletedSatellites();

        // Draw Calls
        void Draw();
        void DrawObjects();
        void DrawSatellites();

        // UI
        void DisplayUI(); // draws User Interface to screen
        void DisplayMainMenu();
        void DisplayControlsUI();
        void DisplaySimInfoUI();
        void DisplayFPSUI();
        void DisplayLaunchUI();
        void DisplaySatellitesUI();
        
        // FPS Tracker
        void UpdateFPS(); // updates the fps counter every frame

        // Main Render Loop
        void RenderLoop(); // Main render loop, core of the render

        // Settings Management
        void EnableWireframeMode();
        void DisableWireframeMode();
        void EnableFaceCulling();
        void DisableFaceCulling();
        void IncreaseSimRate();
        void DecreaseSimRate();
        void ResetSimRate();
        void Pause();
        void Resume();

        // TODO: Implement Later
        void OpenSaveFiles();
        void LoadSaveFile(const char* filename);
        void SaveSaveFile();
};

#endif