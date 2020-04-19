#include "layeredRenderingMesh.h"

int layeredRenderingMesh::mesh_nums = 0;
std::string layeredRenderingMesh::vertex_shader = "Shaders/mesh_vs.glsl";
std::string layeredRenderingMesh::fragment_shader = "Shaders/layered_mesh_fs.glsl";
std::string layeredRenderingMesh::geometry_shader = "Shaders/mesh_gs.glsl";
//GLuint mesh::shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
GLuint layeredRenderingMesh::shader = -1;

glm::vec3 layeredRenderingMesh::fronts[6] = {
	glm::vec3(1,0,0),
	glm::vec3(-1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,-1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,-1),
};


//mesh( mesh_name, meshId, *f,  *p,  *t)
layeredRenderingMesh::layeredRenderingMesh()
{
	update_cameraParameters();
	for (int i = 0; i < 6; i++)
	{
		cubemap_cameras[i] = camera(cubemap_cameraP[i], fronts[i]);
	}
}


layeredRenderingMesh::~layeredRenderingMesh()
{
}

void layeredRenderingMesh::update_cameras()
{
	update_cameraParameters();
	for (int i = 0; i < 6; i++)
	{
		cubemap_cameras[i] = camera(cubemap_cameraP[i], fronts[i]);
	}
}

void layeredRenderingMesh::update_cameraParameters()
{
	glm::vec3 camPos = get_world_pos();
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





layeredRenderingMesh::layeredRenderingMesh(std::string mesh_name, int meshID, fog * f, pointLight * p, transform * t)
{
	this->mesh_name = mesh_name;
	this->meshID = meshID;
	this->f = *f;
	this->p = *p;
	this->t = *t;
	mesh_nums++;
	//initMeshShader();
	initMesh();
}



void layeredRenderingMesh::layered_rendering(int shading_mode, int currentID, glm::mat4 V, glm::mat4 P, glm::vec4 plane, glm::vec3 camPos, int Pass)
{
	glm::mat4 R = glm::rotate(t.rotateAngle, glm::vec3(t.rotateAxis[0], t.rotateAxis[1], t.rotateAxis[2]));
	glm::mat4 T = glm::translate(t.translate);
	glm::mat4 S = glm::scale(t.scale);
	glm::mat4 M = R * S * T;
	glUseProgram(shader);

	glUniform4f(UniformLoc::plane_loc, plane.x, plane.y, plane.z, plane.a);

	glUniformMatrix4fv(UniformLoc::M_loc, 1, false, glm::value_ptr(M));

	glUniformMatrix4fv(UniformLoc::V_loc, 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(UniformLoc::P_loc, 1, false, glm::value_ptr(P));
	glUniform3f(UniformLoc::lightPos_loc, p.lightPos.x, p.lightPos.y, p.lightPos.z);
	glUniform3f(UniformLoc::cameraPos_loc, camPos.x, camPos.y, camPos.z);
	glUniform1i(UniformLoc::pass_loc, Pass);
	glUniform1i(UniformLoc::shadingMode_loc, shading_mode);

	//fog
	glUniform1f(UniformLoc::density_loc, f.density);
	glUniform1f(UniformLoc::gradient_loc, f.gradient);
	glUniform3f(UniformLoc::fogColor_loc, f.fog_color.x, f.fog_color.y, f.fog_color.z);

	//cubemap
	//glUniform1i(UniformLoc::dcubemap_loc, 12);
	//glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, cubemap_id);

	//pick id
	glUniform1i(UniformLoc::meshid_loc, meshID);
	glUniform1i(UniformLoc::currentid_loc, currentID);
	glBindVertexArray(mesh_data.mVao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh_data.DrawMesh();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}



void layeredRenderingMesh::initMesh()
{
	mesh_data = LoadMesh(mesh_name);
}

void layeredRenderingMesh::update(fog *f, pointLight *p, transform *t, GLuint cubemap_id)
{
	this->f = *f;
	this->p = *p;
	this->t = *t;
}

void layeredRenderingMesh::reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	}
	else
	{
		if (shader != -1)
		{
			glDeleteProgram(shader);
		}
		shader = new_shader;
	}

}

void layeredRenderingMesh::init_shader()
{

	shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
}



glm::vec3 layeredRenderingMesh::get_world_pos()
{
	return this->t.translate;
}
// static functions

