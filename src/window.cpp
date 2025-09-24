#define _USE_MATH_DEFINES
#include <cmath>

#include "window.hpp"
#include <iostream>

// Constructor
Window::Window(const char* title, int width, int height, int xPos, int yPos)
: camera(width, height)
{
    windowTitle = title;
    windowWidth = width;
    windowHeight = height;
    windowXPos = xPos;
    windowYPos = yPos;
}

// Window Setup / Shutdown
void Window::Initialize()
{
    // initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW";
    }

    // give GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, antiAiliasingSamples);

    // create GLFW window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    // initialise glad loader
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD loader";
        glfwTerminate();
    }
    
    // setting GLFW callbacks
    glfwSetWindowPos(window, windowXPos, windowYPos);
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // initialise ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::GetStyle();
    ImGui::StyleColorsDark();
    // io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->Fonts->AddFontFromFileTTF("fonts/dejavu-sans/DejavuSans-MdPB.ttf", 14.0f);
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    
    // create shaders after Opengl and dependencies have initialised
    lightShader = std::make_unique<Shader>("shaders\\light.vert", "shaders\\light.frag");
    objectShader = std::make_unique<Shader>("shaders\\object.vert", "shaders\\object.frag");
    lineShader = std::make_unique<Shader>("shaders\\line.vert", "shaders\\line.frag");

    // create objects
    sun = std::make_unique<Light>(109.0f, 32, 16, glm::vec4(253.0f / 255.0f, 251.0f / 255.0f, 211.0f / 255.0f, 1.0f), glm::vec3(0.0f, -23544.2f, 0.0f));
    planet = std::make_unique<Sphere>(1.0f, segmentCount, segmentCount / 2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), SPHERE_COLOR_RGB);

    // place obkects
    sun->Place();
    sun->SendLightInfoToShader(*objectShader);
    planet->Place();

    // enable msaa
    glEnable(GL_MULTISAMPLE);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // setup face culling
    glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

    // enable blending
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Terminate() 
{   
    // delete shaders
    lineShader->Delete();
    objectShader->Delete();
    lightShader->Delete();

    // delete all objects
    DeleteObject();

    // shut down ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // destroy window and end GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

// GLFW Callbacks
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Window* windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->ResizeWindow(width, height);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->KeyInput(key, scancode, action, mods);
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    Window* windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    windowPtr->ScrollInput(xOffset, yOffset);
}

// Input Management
void Window::ResizeWindow(int width, int height)
{
    if (width && height)
    {
        glViewport(0, 0, width, height);
        camera.ScreenSizeUpdate(width, height);
    }
}

void Window::KeyInput(int key, int scancode, int action, int mods)
{
    if (!io->WantCaptureKeyboard)
    {
        if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!fullscreen)
        {
            glfwGetWindowPos(window, &windowXPos, &windowYPos);
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
            fullscreen = true;
        }
        else
        {
            glfwSetWindowMonitor(window, NULL, windowXPos, windowYPos, windowWidth, windowHeight, GLFW_DONT_CARE);
            fullscreen = false;
        }
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        camera.ChangeMode();
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        camera.ResetCamera();
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        IncreaseSimRate();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        DecreaseSimRate();
    }
    if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
    {
        ResetSimRate();
    }
    if (key == GLFW_KEY_PAUSE && action == GLFW_PRESS)
    {
        if (paused) 
        {
            paused = false;
            Resume();
        }
        else 
        {
            paused = true;
            Pause();   
        }
    }
    }
}

void Window::ScrollInput(double xOffset, double yOffset)
{
    if (!io->WantCaptureMouse)
    {
        camera.CameraScrollInput(yOffset);
    }
}

// Object Management
void Window::AddObject() {}

void Window::UpdateObjects()
{
    UpdateSatellites();
}

void Window::DeleteObject()
{
    sun->Delete();
    planet->Delete();
}

// Satellites
void Window::AddSatellites(const char* name, float mass, float longitude, float latitude, float azimuth, float altitude, float velocity, float distance, float FPA, float radius, float color[3])
{
    Satellite satellite = Satellite(name, mass, EARTH_MASS, EARTH_RADIUS, radius, glm::vec4(color[0], color[1], color[2], 1.0f));
    satellite.CalculateOrbitalParameters(longitude, latitude, azimuth, altitude, velocity, distance, FPA, simTime);
    satellites.push_back(satellite);
}

void Window::UpdateSatellites()
{
    for (int i = 0; i < satellites.size(); i++)
    {
        Satellite& satellite = satellites[i];
        satellite.UpdatePosition(simTime);
    }
}

void Window::DeleteSatellites(const char* name)
{
    for (int i = 0; i < satellites.size(); i++)
    {
        Satellite& satellite = satellites[i];
        if (satellite.satelliteName == name)
        {
            satellite.isAlive = false;
        }
    }
}

void Window::TerminateDeletedSatellites()
{
    satellites.erase
    (
        std::remove_if
        (
            satellites.begin(),
            satellites.end(),
            [](const Satellite& satellite){return !satellite.isAlive;}
        ),
        satellites.end()
    );
}

// Draw Calls
void Window::Draw()
{
    DrawObjects();
    DrawSatellites();
}

void Window::DrawObjects()
{
    sun->Draw(*lightShader, camera);
    planet->Draw(*objectShader, camera);
}

void Window::DrawSatellites() 
{
    TerminateDeletedSatellites();
    UpdateSatellites();
    for (int i =0; i < satellites.size(); i++)
    {
        Satellite& satellite = satellites[i];
        satellite.Draw(*lineShader, *objectShader, camera);
    } 
}

// UI
void Window::DisplayUI()
{
    DisplayMainMenu();

    DisplayControlsUI();
    DisplaySimInfoUI();
    DisplayFPSUI();

    // Launch UI
    DisplayLaunchUI();
    DisplaySatellitesUI();
}

void Window::DisplayMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Contols", "", &displayControls);

            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4"))
            {
                glfwSetWindowShouldClose(window, true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug"))
        {
			ImGui::SeparatorText("Draw Debug");
            if (ImGui::MenuItem("Wireframe Mode", "", &wireframeMode))
			{
				if (wireframeMode) EnableWireframeMode();
        		else DisableWireframeMode();
			}
            if (ImGui::MenuItem("Face culling", "", &faceCulling))
			{
				if (faceCulling) EnableFaceCulling();
				else DisableFaceCulling();
			}
			ImGui::SeparatorText("Context Debug");
			ImGui::MenuItem("Display FPS", "", &displayFPS);
            ImGui::MenuItem("Display Sim Info", "", &displaySimInfo);
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Satellites"))
        {
            if (ImGui::MenuItem("Launch"))
            {
                launchUiWindows.push_back(LaunchUI{});
            }
            if (ImGui::BeginMenu("Destroy"))
            {
                for (int i = 0; i < satellites.size(); i++)
                {
                    Satellite& satellite = satellites[i];
                    if (ImGui::MenuItem(satellite.satelliteName.c_str()))
                    {
                        DeleteSatellites(satellite.satelliteName.c_str());
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::MenuItem("Current", "", &displaySatellites);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Window::DisplayControlsUI()
{
    if (displayControls)
    {
        if (ImGui::Begin("Controls", &displayControls))
        {
            ImGui::SeparatorText("Camera Controls");
            ImGui::Text("Reset Camera:"); ImGui::SameLine(); ImGui::TextDisabled("C");
            ImGui::Text("Change Camera Mode:"); ImGui::SameLine(); ImGui::TextDisabled("v");
            ImGui::Text("Look around:"); ImGui::SameLine(); ImGui::TextDisabled("Hold Left Click");
            ImGui::SeparatorText("Movement Controls");
            ImGui::TextDisabled("(Only in free camera mode)");
            ImGui::Text("Forward:"); ImGui::SameLine(); ImGui::TextDisabled("W");
            ImGui::Text("Left:"); ImGui::SameLine(); ImGui::TextDisabled("A");
            ImGui::Text("Backward:"); ImGui::SameLine(); ImGui::TextDisabled("S");
            ImGui::Text("Right:"); ImGui::SameLine(); ImGui::TextDisabled("D");
            ImGui::Text("Up:"); ImGui::SameLine(); ImGui::TextDisabled("Space");
            ImGui::Text("Down:"); ImGui::SameLine(); ImGui::TextDisabled("Ctrl");
            ImGui::Text("Speed:"); ImGui::SameLine(); ImGui::TextDisabled("Shift");
            ImGui::SeparatorText("Sim Controls");
            ImGui::Text("Increase Sim Rate:"); ImGui::SameLine(); ImGui::TextDisabled("Up");
            ImGui::Text("Decrease Sim Rate:"); ImGui::SameLine(); ImGui::TextDisabled("Down");
            ImGui::Text("Reset Sim Rate:"); ImGui::SameLine(); ImGui::TextDisabled("Home");
        }
        ImGui::End();
    }
}

void Window::DisplaySimInfoUI()
{
    if (displaySimInfo)
    {
        if (ImGui::Begin("Sim Info", &displaySimInfo))
        {
            ImGui::Text("Sim Rate: %.2fX", simRate);
            ImGui::Text("Sim Time: %.2fs", simTime);
            ImGui::Separator();
            ImGui::Text("ΔT: %.fs", deltaTime);
            ImGui::Text("Run Time: %.2fs", runTime);
        }
        ImGui::End();
    }
}

void Window::DisplayFPSUI()
{
    if (displayFPS)
    {
        if (ImGui::Begin("FPS stats", &displayFPS))
        {
            ImGui::Text("Current FPS: %.2f", currentFPS);
            ImGui::Text("Average FPS: %.2f", averageFPS);
            ImGui::Text("Min FPS: %.2f", minFps);
        }
        ImGui::End();
    }
}

void Window::DisplayLaunchUI()
{
    for (int i = 0; i < launchUiWindows.size(); i++)
    {
        LaunchUI& launchUI = launchUiWindows[i];
        std::string launchUiTitle = "Launch##" + std::to_string(i + 1);
        if (launchUI.isOpen)
        {
            if (ImGui::Begin(launchUiTitle.c_str(), &launchUI.isOpen))
            {
                ImGui::Text("Name");
                ImGui::InputTextWithHint("##Name", "Name", launchUI.name, 30);
                ImGui::Text("Mass");
                ImGui::InputFloat("kg##Mass", &launchUI.mass_kg);
                ImGui::Text("Radius");
                ImGui::InputFloat("##Radius", &launchUI.radius);
                ImGui::SeparatorText("Launch");
                ImGui::Text("Launch Longitude:");
                ImGui::InputFloat("°##Longitude", &launchUI.longitude_Degrees);
                ImGui::Text("Launch Latitude:");
                ImGui::InputFloat("°##Latitude", &launchUI.latitude_Degrees);
                ImGui::Text("Launch Azimuth:");
                ImGui::InputFloat("°##Azimuth", &launchUI.azimuth_Degrees);
                ImGui::SeparatorText("MECO");
                ImGui::Text("Altiutude:");
                ImGui::InputFloat("km##Altitude", &launchUI.altitude_km);
                ImGui::Text("Velocity:");
                ImGui::InputFloat("m/s##Velocity", &launchUI.velocity_ms);
                ImGui::Text("Downrange Distance:");
                ImGui::InputFloat("km##Distance", &launchUI.downrangeDistance_km);
                ImGui::Text("Flight Path Angle:");
                ImGui::InputFloat("°##FPA", &launchUI.flightPathAngle_Degrees);
                ImGui::Separator();
                ImGui::ColorEdit3("##Color", launchUI.color);
                if (ImGui::Button("Launch!"))
                {
                    launchUI.isOpen = false;
                    AddSatellites(launchUI.name, launchUI.mass_kg, glm::radians(launchUI.longitude_Degrees), glm::radians(launchUI.latitude_Degrees), glm::radians(launchUI.azimuth_Degrees), launchUI.altitude_km * 1000, launchUI.velocity_ms, launchUI.downrangeDistance_km * 1000, glm::radians(launchUI.flightPathAngle_Degrees), launchUI.radius, launchUI.color);
                }
            }
            ImGui::End();
        }
    }

    // Removing UIs from memory once closed
    launchUiWindows.erase
    (
        std::remove_if
        (
            launchUiWindows.begin(),
            launchUiWindows.end(),
            [](const LaunchUI& launchUI){return !launchUI.isOpen;}
        ),
        launchUiWindows.end()
    );
}

void Window::DisplaySatellitesUI()
{
    if (displaySatellites)
    {
        if (ImGui::Begin("Current Satellites", &displaySatellites))
        {
            for (int i = 0; i < satellites.size(); i++)
            {
                Satellite& satellite = satellites[i];
                ImGui::SeparatorText((satellite.satelliteName).c_str());
                ImGui::Text("Mass: %.2fkg", satellite.satelliteMass);
                ImGui::Text("Apoapsis: %.2fkm", satellite.satelliteApoapsisAltitude / 1000.0f);
                ImGui::Text("Periapsis: %.2fkm", satellite.satellitePeriapsisAltitude / 1000.0f);
                ImGui::Text("Eccentricity %.2f", satellite.satelliteEccentricity);
                ImGui::Text("Semi-major axis %.2fkm", satellite.satelliteSemiMajorAxis / 1000.0f);
                ImGui::Text("Inclination %.2f°", glm::degrees(satellite.satelliteInclination));
                ImGui::Text("Orbital Period %.2fs", satellite.satelliteOrbitalPeriod);
                ImGui::Text("Altitude %.2fkm", satellite.satelliteAltitude / 1000.0f);
                ImGui::Text("Velocity %.2fm/s", satellite.satelliteVelocity);
                
            }
            ImGui::End();
        }
    }
}

// FPS Tracker
void Window::UpdateFPS()
{
    fpsTrack.push_back(1.0 / frameTime);
    if (fpsTrack.size() > 30)
    {
        fpsTrack.erase(fpsTrack.begin());
    }

    if (fpsCrntDisplayTime - fpsPrevDisplayTime >= 1.0 / 30.0)
    {
        currentFPS = 1.0 / frameTime;
        double total = 0.0;
        for (int i = 0; i < 30; i++)
        {
            total += fpsTrack[i];
        }
        averageFPS = total / 30;
        if (minFps == -1.0) minFps = 1.0 / frameTime;
        else if ((1.0 / frameTime) < minFps) minFps = 1.0 / frameTime;

        fpsPrevDisplayTime = fpsCrntDisplayTime;
    }
}

// Main Render Loop
void Window::RenderLoop() 
{
    // set time tracking variables
    double prevTime = glfwGetTime();
    double crntTime;
    fpsPrevDisplayTime = prevTime;
    accumulator = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        // clear buffers and tell imgui this is a new frame
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		
        // Input polling and UI
        glfwPollEvents();
        if (!io->WantCaptureMouse)
        {
            camera.CameraMouseInput(window);
        }
        if (!io->WantCaptureKeyboard)
        {
            camera.CameraKeyInput(window);
        }

        // Update Camera
        camera.UpdateMatrix(FOV, nearPlaneDist, farPlaneDist);

        // Draw UI
        DisplayUI();

        // calculate time deltas for physics
        double crntTime = glfwGetTime();
        fpsCrntDisplayTime = crntTime;

        frameTime = crntTime - prevTime;
        prevTime = crntTime;

        accumulator += frameTime;
        runTime += frameTime;
        
        // fps tracker
        UpdateFPS();

        // physics updater
        while (accumulator >= deltaTime)
        {
            UpdateObjects();
            accumulator -= deltaTime;
			simTime += (deltaTime * simRate);
        }

        // render
        Draw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

// Settings Management
void Window::EnableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Window::DisableWireframeMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::EnableFaceCulling()
{
    glEnable(GL_CULL_FACE);
}

void Window::DisableFaceCulling()
{
    glDisable(GL_CULL_FACE);
}

void Window::IncreaseSimRate()
{
    double newSimRate = simRate * 10.0;
    if (newSimRate <= 1000000.0)
    {
        simRate = newSimRate;
    }
}

void Window::DecreaseSimRate()
{
    double newSimRate = simRate / 10.0;
    if (newSimRate >= 1.0)
    {
        simRate = newSimRate;
    }
}

void Window::ResetSimRate()
{
    simRate = 1.0;
}

void Window::Pause()
{
    simRate = 0.0;
}

void Window::Resume()
{
    ResetSimRate();
}