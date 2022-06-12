#include "camera.h"

const float Camera::DEFAULT_YAW = -90.0f;
const float Camera::DEFAULT_PITCH = 0.0f;
const float Camera::DEFAULT_SPEED = 2.5f;
const float Camera::DEFAULT_SENSITIVITY = 0.1f;
const float Camera::DEFAULT_ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(DEFAULT_SPEED), mouseSensitivity(DEFAULT_SENSITIVITY), zoom(DEFAULT_ZOOM)
{
	position = position;
	worldUp = up;
	yaw = yaw;
	pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch)
{}

glm::mat4 Camera::getViewMatrix()
{
	printf("%f %f %f | %f %f %f | %f %f %f\n", position.x, position.y, position.z, front.x, front.y, front.z, up.x, up.y, up.z);
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == Movement::FORWARD)
	{
		position += front * velocity;
	}
	if (direction == Movement::BACKWARD)
	{
		position -= front * velocity;
	}
	if (direction == Movement::LEFT)
	{
		position -= right * velocity;
	}
	if (direction == Movement::RIGHT)
	{
		position += right * velocity;
	}
	if (direction == Movement::UP)
	{
		position += up * velocity;
	}
	if (direction == Movement::DOWN)
	{
		position -= up * velocity;
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);

	right = glm::normalize(glm::cross(this->front, worldUp));
	up = glm::normalize(glm::cross(right, this->front));
}