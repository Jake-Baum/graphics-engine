#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera
{
public:
	static enum class Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
				 glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
				 float yaw = DEFAULT_YAW,
				 float pitch = DEFAULT_PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 getViewMatrix();
	void processKeyboard(Movement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);

private:
	static const float DEFAULT_YAW;
	static const float DEFAULT_PITCH;
	static const float DEFAULT_SPEED;
	static const float DEFAULT_SENSITIVITY;
	static const float DEFAULT_ZOOM;

	void updateCameraVectors();
};
#endif