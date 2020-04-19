#include "mousePicking.h"
 int mousePicking::RECURSION_COUNT = 100;
 float mousePicking::RAY_RANGE = 1000.0;



mousePicking::mousePicking()
{
}


mousePicking::~mousePicking()
{
}


mousePicking::mousePicking(int window_w, int window_h, camera* cam, float terrain_scale , glm::mat4 projection) {
	//scale = t->get_scale();
	this->scale = terrain_scale;
	this->window_w = window_w;
	this->window_h = window_h;
	this->cam = *cam;
	this->terrain_w = 100;
	this->terrain_h = 100;
	//picking_terrain = *t;
	//this->picking_terrain = *t;

	projectionMatrix = projection;
	viewMatrix = this->cam.get_view_matrice();
	//this.terrain = terrain;
}

 glm::vec3 mousePicking::getCurrentTerrainPoint() {
	return currentTerrainPoint;
}

 glm::vec3 mousePicking::getCurrentRay() {
	return currentRay;
}

 void mousePicking::update(int x, int y, camera* c ,terrain* t ) {
	 // should I change to pointer?
	//viewMatrix = c->get_view_matrice();
	 this->cam = *c  ;
	currentRay = calculateMouseRay(x,y);
	glm::vec3 endpoints = getPointOnRay(currentRay, RAY_RANGE);
	glm::vec3 camerapos = c->get_camera_position();
	//std::cout << "endpoint" << endpoints.x << "    " << endpoints.y << "    " << endpoints.z << std::endl;
	//std::cout << "camerapos" << camerapos.x << "    " << camerapos.y << "    " << camerapos.z << std::endl;
	if ( intersectionInRange(t,0, RAY_RANGE, currentRay)) {
		currentTerrainPoint = binarySearch(t,0, 0, RAY_RANGE, currentRay);
		// std::cout << "terrain pos" << currentTerrainPoint.x << "    " << currentTerrainPoint.y << "    " << currentTerrainPoint.z << std::endl;
		//std::cout << "terrain pos" << currentTerrainPoint.x << "    " << currentTerrainPoint.y << "    " << currentTerrainPoint.z << std::endl;
		//std::cout << "test";
	}
	else {
		currentTerrainPoint = glm::vec3(9999.0);
		std::cout << " not in range" << std::endl;
	}
	

}

 glm::vec3 mousePicking::calculateMouseRay(int x, int y) {
	 float mouseX = float(x);
	 float mouseY = float(y);
	glm::vec2 normalizedCoords = getNormalisedDeviceCoordinates(mouseX, mouseY);
	glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);

	return worldRay;
}

 glm::vec3 mousePicking::toWorldCoords(glm::vec4 eyeCoords) {
	 glm::mat4 invertedView = glm::inverse(this->cam.get_view_matrice());
	 glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);
	return mouseRay;
}

 glm::vec4 mousePicking::toEyeCoords(glm::vec4 clipCoords) {
	glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * clipCoords;
	return  glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

 glm::vec2 mousePicking::getNormalisedDeviceCoordinates(float mouseX, float mouseY) {
	float x = (2.0f * mouseX) /float( window_w) - 1.0f;
	float y = (2.0f * mouseY) / float(window_h) - 1.0f;
	return  glm::vec2(x, -y);
}


  glm::vec3  mousePicking::getPointOnRay(glm::vec3 ray, float distance) {
	 glm::vec3 camPos = cam.get_camera_position();
	 glm::vec3 start =  glm::vec3(camPos.x, camPos.y, camPos.z);
	 glm::vec3 scaledRay =  glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
	 return start+ scaledRay;
 }


  glm::vec3  mousePicking::binarySearch(terrain* t, int count, float start, float finish, glm::vec3 ray) {
	 float half = start + ((finish - start) / 2.0f);
	 //std::cout << "test2" ;
	 if (count >= RECURSION_COUNT) {
		 glm::vec3 endPoint = getPointOnRay(ray, half);
		 return endPoint;
		 //Terrain terrain = getTerrain(endPoint.getX(), endPoint.getZ());
		 //if (terrain != null) {
			// return endPoint;
		 //}
		 //else {
			// return null;
		// }
	 }
	 if (intersectionInRange(t, start, half, ray)) {
		 return binarySearch( t, count + 1, start, half, ray);
	 }
	 else {
		 return binarySearch( t, count + 1, half, finish, ray);
	 }
 }

  bool  mousePicking::intersectionInRange(terrain* t, float start, float finish, glm::vec3 ray) {
	  float half = start + ((finish - start) / 2.0f);
	 glm::vec3 startPoint = getPointOnRay(ray, start);

	 glm::vec3 endPoint = getPointOnRay(ray, finish);
	 glm::vec3 midPoint = getPointOnRay(ray, half);
	 //std::cout << "start point" << startPoint.x << "     "<<startPoint.y << "     " << startPoint.z << std::endl;
	 //std::cout << "end point" << endPoint.x << "     "<< endPoint.y << "     " << endPoint.z << std::endl;
	 bool startInRange = inTerrainRange(int(startPoint.x/scale), int(startPoint.z / scale));
	 //bool endInRange = inTerrainRange(int(endPoint.x / scale), int(endPoint.z / scale));
	 bool midInRange = inTerrainRange(int(midPoint.x / scale), int(midPoint.z / scale));
	 if (!isUnderGround(t,startPoint) && isUnderGround(t,endPoint)&&startInRange&&midInRange) {
		 return true;
	 }
	 else {
		 return false;
		 std::cout << " not in terrain range!" << std::endl;
	 }
 }

  bool  mousePicking::isUnderGround(terrain* t, glm::vec3 testPoint) {
	  //return true;
	 //Terrain terrain = getTerrain(testPoint.getX(), testPoint.getZ());
	 float height = scale*t->get_height(int(testPoint.x/scale),int(testPoint.z/scale));

	 if (testPoint.y < height) {
		 return true;
	 }
	 else {
		 return false;
	 }
 }

  bool mousePicking::inTerrainRange(int x, int z)
  {
	  if (x > -terrain_w * 0.5&&x < terrain_w*0.5&&z>-terrain_h * 0.5&&z < terrain_h*0.5)

		  return true;
	  else
		  return false;
  }



