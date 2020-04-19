#include "camera.h"



camera::camera()
{
}
camera::camera(cameraP cP)
{
	position = cP.start_position;
	world_up = cP.start_world_up;
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	pitch = cP.start_pitch;
	yaw = cP.start_yaw;
	key_speed = cP.start_key_speed;
	mouse_speed = cP.start_mouse_speed;
	update();
}

camera::camera(cameraP cP, glm::vec3 front)
{
	position = cP.start_position;
	world_up = cP.start_world_up;
	this->front = front;
	pitch = cP.start_pitch;
	yaw = cP.start_yaw;
	key_speed = cP.start_key_speed;
	mouse_speed = cP.start_mouse_speed;
	update();
}

void camera::invert_pitch()
{
	this->pitch = -this->pitch;
}

void camera::set_camera_position(glm::vec3 position)
{
	this->position = position;
}

void camera::move_camera(char key, float delta_time)
{
	float velocity = key_speed * delta_time;

	switch (key)
	{
	// camera move.
	case 'w':
	case 'W':
		position += velocity * front;
		break;

	case 's':
	case 'S':
		position -= velocity * front;
		break;

	case 'a':
	case 'A':
		position -= glm::normalize(glm::cross(front, up)) * velocity;
		break;

	case 'd':
	case 'D':
		position += glm::normalize(glm::cross(front, up)) * velocity;
		break;
	}
	update();
}

void camera::turn_camera(float x_change, float y_change)
{
	x_change *= -mouse_speed;
	y_change *= mouse_speed;
	yaw += x_change;
	pitch += y_change;
	if (pitch > 89.9f)
	{
		pitch = 89.9f;
	}

	if (pitch < -89.9f)
	{
		pitch = -89.9f;
	}


	//std::cout << "pitch : " << pitch << "     yaw : " << yaw << std::endl;

	update();
}

glm::vec2  camera:: get_yp()
{
	return glm::vec2(yaw, pitch);

}

void camera::set_yp(glm::vec2 yp)
{
	this->yaw = yp.x;
	this->pitch = yp.y;
	update();
}

void camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, world_up));
	up = glm::normalize(glm::cross(right, front));

	//std::cout << right.x << " " << right.y << " " << right.z<<std::endl;
}

glm::vec3 camera::get_camera_position()
{
	return position;
	//return glm::vec3(yaw, pitch, 0.0);
}

glm::vec3 camera::get_camera_front()
{
	return glm::normalize(front);
}

glm::mat4 camera::get_view_matrice()
{
	return glm::lookAt(position, position + front, up);
}

camera::~camera()
{
}

glm::vec3 camera::get_camera_right()
{
	return right;
}

glm::vec3 camera::get_camera_up()
{
	return up;
}
