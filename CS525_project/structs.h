#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>



struct pointLight
{
public :
	float Ka;
	float Kd;
	float Ks;

	glm::vec3 La;
	glm::vec3 Ld;
	glm::vec3 Ls;
	glm::vec3 lightPos;
};

struct directionalLight
{
public:
	float Ka;
	float Kd;

	glm::vec3 La;
	glm::vec3 Ld;

	glm::vec3 dir;
};

struct fog
{
public :
	float density;
	float gradient;
	glm::vec3 fog_color;
};

struct waterP
{
public :
	float water_height;
	float wave_strength;
	float wave_speed;
	glm::vec3 water_color;
	float water_color_mix;
	float shineness;
	float wave_length;
	float wave_amplitude;
	float edge_softness;
	float glitch_offset;
	float minBlue;
	float maxBlue;
	float murkyDepth;

};

struct quadP
{
public:
	int N;
	int repeat;// what repeat???
};

struct terrainP
{
	float height_level;
	int seed;
	float smooth;
	float roughness;
	int frequency;

	glm::vec3 peak_color;
	float peak_height;
	glm::vec3 highpart_color;
	float high_height;
	glm::vec3 middlepart_color;
	float middle_height;
	glm::vec3 underwater_color;
	float under_height;
};

struct transform
{
public :
	glm::vec3 translate;
	glm::vec3 scale;
	float rotateAngle;
	int rotateAxis[3];
};

struct cameraP
{
public :
	glm::vec3 start_position;
	glm::vec3 start_world_up;
	float start_pitch;
	float start_yaw;
	float start_key_speed;
	float start_mouse_speed;

};

struct sunP
{
public:
	glm::vec2 sun_scale;
	glm::vec3 sun_pos;
};

struct lensFlareP
{
	float strength;
	float scale[9];
	float fadefactor;
};




#endif

