#include "window.hpp"

Window::Window
(
    const char* title,
    int width,
    int height,
    int xPos,
    int yPos
):
camera(width, height)
{
    windowTitle = title;
    windowWidth = width;
    windowHeight = height;
    windowXPos = xPos;
    windowYPos = yPos;
}

void Window::Initialize()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, MSAAsamples);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD loader";
        glfwTerminate();
    }

    
    glfwSetWindowPos(window, windowXPos, windowYPos);
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::GetStyle();
    ImGui::StyleColorsDark();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->Fonts->AddFontFromFileTTF("fonts/dejavu-sans/DejavuSans-MdPB.ttf", 14.0f);
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    lightShader = std::make_unique<Shader>("shaders\\light.vert", "shaders\\light.frag");
    shaderProgram = std::make_unique<Shader>("shaders\\object.vert", "shaders\\object.frag");
    sun = std::make_unique<Light>(109.0f, 32, 16, glm::vec4(253.0f / 255.0f, 251.0f / 255.0f, 211.0f / 255.0f, 1.0f), glm::vec3(0.0f, -23544.2f, 0.0f));
    planet = std::make_unique<Sphere>(1.0f, segmentCount, segmentCount / 2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), SPHERE_COLOR_RGB);

    sun->Place();
    sun->SendLightInfoToShader(*shaderProgram);
    planet->Place();
}

void Window::Terminate() 
{   
    DeleteObject();
    DeleteShader();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

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
}

void Window::ScrollInput(double xOffset, double yOffset)
{
    if (!io->WantCaptureMouse)
    {
        camera.CameraScrollInput(yOffset);
    }
}

void Window::OpenSaveFiles() {}
void Window::LoadSaveFile(const char* filename) {}
void Window::SaveSaveFile() {}

void Window::AddObject(ObjectType type) {}
void Window::UpdateObjects() {}

void Window::DrawObjects()
{
    sun->Draw(*lightShader, camera);
    planet->Draw(*shaderProgram, camera);
}

void Window::DeleteObject()
{
    sun->Delete();
    planet->Delete();
}

void Window::AddShader(const char* vertexFile, const char* fragmentFile) {}

void Window::DeleteShader()
{
    lightShader->Delete();
    shaderProgram->Delete();
}

void Window::DrawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Contols", "", &displayControls);
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

        ImGui::EndMainMenuBar();
    }

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

void Window::RenderLoop() 
{
    double prevTime = glfwGetTime();
    double fpsPrevUpdateTime = prevTime;
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        // clear buffers and tell imgui this is a new frame
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
        camera.CameraKeyInput(window);

        camera.UpdateMatrix(FOV, nearPlaneDist, farPlaneDist);
        DrawUI();

        // calculate time deltas for physics
        double crntTime = glfwGetTime();
        double fpsCrntUpdateTime = crntTime;
        frameTime = crntTime - prevTime;
        prevTime = crntTime;
        accumulator += frameTime;
        runTime += frameTime;

        fpsTrack.push_back(1.0 / frameTime);
        if (fpsTrack.size() > 30)
        {
            fpsTrack.erase(fpsTrack.begin());
        }

        if (fpsCrntUpdateTime - fpsPrevUpdateTime >= 1.0 / 30.0)
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

            fpsPrevUpdateTime = fpsCrntUpdateTime;
        }

        // physics updater
        while (accumulator >= deltaTime)
        {
            UpdateObjects();
            accumulator -= deltaTime;
			simTime += (deltaTime * simRate);
        }

        // render
        DrawObjects();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

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
