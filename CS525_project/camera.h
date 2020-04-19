#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "structs.h"

//this class mainly construct a  V matrix.glm::mat4 get_view_matrice();

class camera
{
public:
	camera();

	//constructor
	camera(cameraP cP);
	// overloading constructor for cube maps
	camera(cameraP cP,glm::vec3 front);

	void move_camera(char key , float delta_time);
	void turn_camera(float x_change, float y_change);
	void invert_pitch();
	void set_camera_position(glm::vec3 position);

	glm::vec3 get_camera_position();

	glm::vec3 get_camera_front();
	glm::vec3 get_camera_right();
	glm::vec3 get_camera_up();

	glm::mat4 get_view_matrice();
	glm::vec2 get_yp();
	void set_yp(glm::vec2 yp);
	

	~camera();

private:

	cameraP cP;
	void update();
	float pitch, yaw, key_speed, mouse_speed;

	glm::vec3 position, front, right, up, world_up;

};

#endif 

