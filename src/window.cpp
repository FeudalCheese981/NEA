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

bool Window::Initialize()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW";
        return false;
    }

    glfwWindowHint(GLFW_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window";
        glfwTerminate();
        return false;
    }

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD loader";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Window::Terminate() 
{
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
}

void Window::ScrollInput(double xOffset, double yOffset)
{
    camera.ScrollInput(yOffset);
}

void Window::OpenSaveFiles() {}
void Window::LoadSaveFile(const char* filename) {}
void Window::SaveSaveFile() {}

void Window::AddObject(ObjectType type) {}
void Window::UpdateObjects() {}
void Window::DrawObjects() {}
void Window::DeleteObject() {}

void Window::AddShader(const char* vertexFile, const char* fragmentFile) {}
void Window::DeleteShader() {}

void Window::DrawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            if (ImGui::MenuItem("Wireframe Mode", "", &wireframeMode))
            {
                if (wireframeMode) DisableWireframeMode();
                else EnableWireframeMode();
            }
            if (ImGui::MenuItem("Face culling", "", &faceCulling))
            {
                if (faceCulling) DisableFaceCulling();
                else EnableFaceCulling();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
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

void Window::RenderLoop() 
{
    double prevTime = glfwGetTime();
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
        DrawUI();

        // calculate time deltas for physics
        double crntTime = glfwGetTime();
        double frameTime = crntTime - prevTime;
        prevTime = crntTime;
        accumulator += frameTime;

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