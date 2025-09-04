#include "camera.hpp"

Camera::Camera(int width, int height)
{
	Camera::width = width;
	Camera::height = height;
	ResetCamera();
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(FOVdeg), ((float)width / (float)height), nearPlane, farPlane);
	
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::CameraInputs(GLFWwindow* window)
{
	if (mode == CAMERA_MODE_FREE)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += speed * orientation;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position += speed * -glm::normalize(glm::cross(orientation, up));
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position += speed * -orientation;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += speed * glm::normalize(glm::cross(orientation, up));
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			position += speed * up;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			position += speed * -up;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			speed = DEFAULT_SPEED * 10;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			speed = DEFAULT_SPEED;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (firstClick)
			{
				glfwSetCursorPos(window, mouseClickX, mouseClickY);
				firstClick = false;
			}

			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotY = sensitivity * (float)(mouseY - mouseClickY) / (float)height;
			float rotX = sensitivity * (float)(mouseX - mouseClickX) / (float)width;

			glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotY), glm::normalize(glm::cross(orientation, up)));

			if (abs(glm::angle(glm::normalize(newOrientation), up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				orientation = newOrientation;
			}

			orientation = glm::rotate(orientation, glm::radians(-rotX), up);

			glfwSetCursorPos(window, mouseClickX, mouseClickY);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
			glfwGetCursorPos(window, &mouseClickX, &mouseClickY);
		}
	}
	else if (mode == CAMERA_MODE_ORBITAL)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (firstClick)
			{
				glfwSetCursorPos(window, mouseClickX, mouseClickY);
				firstClick = false;
			}

			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotX = sensitivity * (float)((mouseY - mouseClickY) / height);
			float rotY = sensitivity * (float)((mouseX - mouseClickX) / width);

			glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));
			glm::vec3 newPosition = glm::rotate(position, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));
			if (abs(glm::angle(glm::normalize(newOrientation), up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				orientation = newOrientation;
				position = newPosition;
			}

			orientation = glm::rotate(orientation, glm::radians(-rotY), up);
			position = glm::rotate(position, glm::radians(-rotY), up);

			glfwSetCursorPos(window, mouseClickX, mouseClickY);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
			glfwGetCursorPos(window, &mouseClickX, &mouseClickY);
		}
	}
}

void Camera::ScrollInput(double yOffset)
{
	if (mode == CAMERA_MODE_ORBITAL)
	{
		glm::vec3 newPosition = position + (float)yOffset * orientation * glm::length(position) / 10.0f;
		if (glm::length(newPosition) >= 2.0f)
		{
			position = newPosition;
		}
		else
		{
			position = glm::normalize(position) * 2.0f;
		}
	}
	
}

void Camera::ChangeMode()
{
	if (mode == CAMERA_MODE_ORBITAL) 
	{
		mode = CAMERA_MODE_FREE;
		positionStoreFree = position;
		orientationStoreFree = orientation;
		position = positionStoreOrbital;
		orientation = orientationStoreOrbital;
	}
	else if (mode == CAMERA_MODE_FREE)
	{
		mode = CAMERA_MODE_ORBITAL;
		positionStoreOrbital = position;
		orientationStoreOrbital = orientation;
		position = positionStoreFree;
		orientation = orientationStoreFree;
	} 
}

void Camera::ResetCamera()
{
	position = glm::vec3(2.0f, 0.0f, 0.0f);
	orientation = -position;
	positionStoreFree = position;
	orientationStoreFree = orientation;
	positionStoreOrbital = position;
	orientationStoreOrbital = orientation;
}