#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.hpp"

enum CameraMode { FREE, ORBITAL };
const glm::vec3 DEFAULT_POS = glm::vec3(2.0f, 0.0f, 0.0f);

class Camera
{
	public:
		glm::vec3 position;
		glm::vec3 orientation;
		glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		glm::vec3 positionStoreFree;
		glm::vec3 orientationStoreFree;

		glm::vec3 positionStoreOrbital;
		glm::vec3 orientationStoreOrbital;

		const float DEFAULT_SPEED = 0.01f;
		
		bool firstClick = true;
		double mouseClickX = 0.0;
		double mouseClickY = 0.0;

		int windowWidth;
		int windowHeight;

		float speed = DEFAULT_SPEED;
		float sensitivity = 150.0f;

		int mode = FREE;

		Camera(int width, int height);

		void ScreenSizeUpdate(int width, int height);
		void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
		void Matrix(Shader& shader, const char* uniform);
		void CameraInputs(GLFWwindow* window);
		void ScrollInput(double yOffset);
		void ChangeMode();
		void ResetCamera();
};

#endif