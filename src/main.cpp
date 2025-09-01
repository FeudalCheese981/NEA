#define _USE_MATH_DEFINES
#include <cmath>

#include "orbit.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include "grid.hpp"

// GLFW & window settings
const unsigned int OPENGL_VERSION_MAJOR = 4;
const unsigned int OPENGL_VERSION_MINOR = 6;
const unsigned int SCREEN_WIDTH = 720;
const unsigned int SCREEN_HEIGHT = 720;
const unsigned int MSAA_SAMPLES = 16;
const unsigned int SEGMENT_COUNT = 1024;

int windowXPos, windowYPos;
int windowWidth, windowHeight;
bool fullscreen = false;

Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (width && height)
	{
		glViewport(0, 0, width, height);
		camera.width = width;
		camera.height = height;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ScrollInput(yOffset);
}

int main()
{
	glfwInit();

	// GLFW hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);

	// GLFW window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "NEA OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context";
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// shader
	Shader lineShader = Shader("shaders\\line.vert", "shaders\\line.frag");
	Shader lightShader = Shader("shaders\\light.vert", "shaders\\light.frag");
	Shader objectShader = Shader("shaders\\object.vert", "shaders\\object.frag");

	// planet
	Sphere planet(1.0f, SEGMENT_COUNT, SEGMENT_COUNT / 2, glm::vec4(0.5f, 0.75f, 0.75f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), SPHERE_COLOR_RGB);
	planet.Place(objectShader);
	
	Sphere moon(1.0f, SEGMENT_COUNT, SEGMENT_COUNT / 2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -100.0f, 0.0f), SPHERE_COLOR_DEFAULT);
	moon.Place(objectShader);

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
	orbitLine.Place(lineShader);
	
	// grid
	Grid gridDark(1000.0f, 100, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	gridDark.Place(lineShader);

	Grid gridBright(1000.0f, 10, glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	gridBright.Place(lineShader);

	Grid gridFull(1000.0f, 2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	gridFull.Place(lineShader);

	// light
	glm::vec3 sunPos = glm::vec3(0.0f, -23544.2f, 0.0f);
	// sunPos = glm::rotate(sunPos, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Light sun(109.3f, 32, 16, glm::vec4(253.0f / 255.0f, 251.0f / 255.0f, 211.0f / 255.0f, 1.0f), sunPos);
	sun.Place(lightShader);
	sun.SendLightInfoToShader(objectShader);

	//
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	double prevTime = glfwGetTime();
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.CameraInputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100000.0f);

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1.0 / 200.0)
		{
			glm::vec3 pos = sun.objectPos;
			// glm::vec3 axis = glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(24.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
			pos = glm::rotate(pos, glm::radians(1.0f / 60.0f), axis);
			sun.objectPos = pos;
			sun.Place(lightShader);
			sun.SendLightInfoToShader(objectShader);

			glm::vec3 moonPos = moon.objectPos;
			moonPos = glm::rotate(moonPos, glm::radians(1.0f / (60.0f * 28.0f)), axis);
			moon.objectPos = moonPos;

			moon.Place(objectShader);

			crntTime = prevTime;
		}

		sun.Draw(lightShader, camera);
		
		planet.Draw(objectShader, camera);
		moon.Draw(objectShader, camera);

		orbitLine.Draw(lineShader, camera, 3.0f);
		gridDark.Draw(lineShader, camera);
		gridBright.Draw(lineShader, camera, 2.0f);
		gridFull.Draw(lineShader, camera, 3.0f);	

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	lineShader.Delete();
	lightShader.Delete();
	objectShader.Delete();

	planet.Delete();
	moon.Delete();
	orbitLine.Delete();
	gridDark.Delete();
	gridBright.Delete();
	gridFull.Delete();
	sun.Delete();
	

	// end of program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}