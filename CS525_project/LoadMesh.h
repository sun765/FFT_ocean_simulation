#ifndef __LOADMESH_H__
#define __LOADMESH_H__


#include <string>
#include <vector>
#include <GL/glew.h>
#include "assimp/Scene.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "location.h"

struct SubmeshData
{
   unsigned int mNumIndices;
   unsigned int mBaseIndex;
   unsigned int mBaseVertex;

   SubmeshData() : mNumIndices(0), mBaseIndex(0), mBaseVertex(0) {}
   void DrawSubmesh();
};

struct MeshData
{
   unsigned int mVao;
   unsigned int mVboVerts;
   unsigned int mVboNormals;
   unsigned int mVboTexCoords;
   unsigned int mIndexBuffer;
   float mScaleFactor; //TODO replace with bounding box

   const aiScene* mScene;
   aiVector3D mBbMin, mBbMax;

   std::vector<SubmeshData> mSubmesh;
   std::string mFilename;

   MeshData() : mVao(-1), mVboVerts(-1), mVboNormals(-1), mVboTexCoords(-1), mIndexBuffer(-1), mScaleFactor(0.0f), mScene(NULL) {}

   void DrawMesh();

};



MeshData LoadMesh(const std::string& pFile);

//void draw_mesh(MeshData mesh_data, GLuint shader, glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::vec4 plane, glm::vec3 lightPos, glm::vec3 camPos);



#endif