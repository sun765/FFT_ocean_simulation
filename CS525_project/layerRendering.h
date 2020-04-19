#ifndef __LAYERRENDERING_H__
#define __LAYERRENDERING_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "structs.h"
#include "location.h"
#include "camera.h"

class layerRendering
{
public:
	layerRendering();
	~layerRendering();

	GLuint cubemap_id;
	void update();
	camera* get_view_matrice();


private:
	GLuint cubemap_id;
	camera cubemap_cameras[6];
	cameraP cubemap_cameraP[6];
	static glm::vec3 fronts[6];

	void update_cameras();
	void update_cameraParameters();

};

#endif

