#include "layerRendering.h"

 glm::vec3 layerRendering::fronts[6] = {
	 glm::vec3(1,0,0),
	 glm::vec3(-1,0,0),
	 glm::vec3(0,1,0),
	 glm::vec3(0,-1,0),
	 glm::vec3(0,0,1),
	 glm::vec3(0,0,-1),
 };

 layerRendering::layerRendering()
 {
	 
	 update_cameraParameters();
	 update_cameras();
 }




layerRendering::~layerRendering()
{
}


void layerRendering::update()
{
	this->cam = *cam;
	this->sP = *sP;
}

void layerRendering::update_cameras()
{
	for (int i = 0; i < 6; i++)
	{
		cubemap_cameras[i] = camera(cubemap_cameraP[i],fronts[i]);
	}
}

void layerRendering::update_cameraParameters()
{
	glm::vec3 camPos = m.get_world_pos();
	for (int i = 0; i < 6; i++)
	{
		cubemap_cameraP[i] =
		{
				camPos,
				glm::vec3(0.0f, 1.0f, 0.0f),
				0.0f,
				0.0f,
				50.0f,
				0.50f
		};
	}
}


camera * layerRendering::get_view_matrice()
{
	return nullptr;
}