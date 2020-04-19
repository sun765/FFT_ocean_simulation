#ifndef __MOUSEPICKING_H__
#define __MOUSEPICKING_H__



#include <windows.h>
#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>

#include "structs.h"
#include "location.h"
#include "InitShader.h"
#include "camera.h"
#include "terrain.h"
class mousePicking
{
public:
	 mousePicking();
	 ~mousePicking();

	 mousePicking(int window_w,int window_h,camera* cam, float terrain_scale,  glm::mat4 projection);

	 glm::vec3 getCurrentTerrainPoint();

	 glm::vec3 getCurrentRay();

	 void update(int x, int y, camera* cam, terrain* t );
	// terrain t;




private:

	 int window_w, window_h;
	 int terrain_w, terrain_h;
	 static  int RECURSION_COUNT;
	 static  float RAY_RANGE;
	 float scale = 10.0;

	 glm::vec3 currentRay ;

	 glm::mat4 projectionMatrix;
	 glm::mat4 viewMatrix;
	 camera cam;
	 terrain picking_terrain;
	 

	  glm::vec3 currentTerrainPoint;

	  glm::vec3 calculateMouseRay(int x, int y);
	  glm::vec3 toWorldCoords(glm::vec4 eyeCoords);

	  glm::vec4 toEyeCoords(glm::vec4 clipCoords);

	  glm::vec2 getNormalisedDeviceCoordinates(float mouseX, float mouseY);

	  glm::vec3 getPointOnRay(glm::vec3 ray, float distance);

	  glm::vec3 binarySearch(terrain* t, int count, float start, float finish, glm::vec3 ray);

	  bool intersectionInRange(terrain* t, float start, float finish, glm::vec3 ray);

	  bool isUnderGround(terrain* t, glm::vec3 testPoint);

	  bool inTerrainRange(int x, int z);

	  // terrain getTerrain(float worldX, float worldZ);
};

#endif
