#include "mesh.h"

int mesh::mesh_nums = 0;
std::string mesh::vertex_shader = "Shaders/mesh_vs.glsl";
std::string mesh::fragment_shader = "Shaders/mesh_fs.glsl";
std::string mesh::geometry_shader = "Shaders/mesh_gs.glsl";
std::string mesh::tessellation_control_shader = "Shaders/mesh_tc.glsl";
std::string mesh::tessellation_evaluation_shader = "Shaders/mesh_te.glsl";
//GLuint mesh::shader = InitShader(vertex_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
GLuint mesh::shader = -1;

mesh::mesh()
{
}
mesh::mesh(std::string mesh_name, int meshID, fog * f, pointLight * p, directionalLight *dl, transform * t, glm::vec2 level)
{
	this->mesh_name = mesh_name;
	this->meshID = meshID;
	this->f = *f;
	this->p = *p;
	this->t = *t;
	this->dl = *dl;
	this->level = level;
	mesh_nums++;
	this->sub_mesh_num = get_submesh_num();
	//initMeshShader();
	initMesh();
}

mesh::~mesh()
{
}

void mesh::draw_mesh(int shading_mode, int currentID, glm::mat4 V, glm::mat4 P, glm::vec4 plane, glm::vec3 camPos, int Pass, directionalLight *dl,  fog *f, bool mode, glm::vec2 level, std::vector<glm::vec3> sub_color,  float time,transform *t)
{
	glm::mat4 R = glm::rotate(t->rotateAngle, glm::vec3(t->rotateAxis[0], t->rotateAxis[1], t->rotateAxis[2]));
	glm::mat4 T = glm::translate(t->translate);
	glm::mat4 S = glm::scale(t->scale);
	glm::mat4 M = R * T * S;
	this->M = M;
	glUseProgram(shader); 
	glUniform1i(UniformLoc::meshhighlight_loc, int(this->hight_light));

	glUniform4f(UniformLoc::plane_loc, plane.x, plane.y, plane.z, plane.a);

	glUniformMatrix4fv(UniformLoc::M_loc, 1, false, glm::value_ptr(M));

	glUniformMatrix4fv(UniformLoc::V_loc, 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(UniformLoc::P_loc, 1, false, glm::value_ptr(P));
	glUniform3f(UniformLoc::lightPos_loc, p.lightPos.x, p.lightPos.y, p.lightPos.z);
	glUniform3f(UniformLoc::cameraPos_loc, camPos.x, camPos.y, camPos.z);
	glUniform1i(UniformLoc::pass_loc, Pass);
	glUniform1i(UniformLoc::shadingMode_loc, shading_mode);

	//direction light
	glUniform3f(UniformLoc::dir_loc, dl->dir.x, dl->dir.y, dl->dir.z);
	glUniform1f(UniformLoc::dka_loc, dl->Ka);
	glUniform1f(UniformLoc::dkd_loc, dl->Kd);
	glUniform3f(UniformLoc::dLa_loc, dl->La.x, dl->La.y, dl->La.z);
	glUniform3f(UniformLoc::dLd_loc, dl->Ld.x, dl->Ld.y, dl->Ld.z);
	
	//fog
	glUniform1f(UniformLoc::density_loc, f->density);
	glUniform1f(UniformLoc::gradient_loc, f->gradient);
	glUniform3f(UniformLoc::fogColor_loc, f->fog_color.x, f->fog_color.y, f->fog_color.z);


	glUniform1f(UniformLoc::time_loc, time);
	//tess
	glUniform2f(UniformLoc::level_loc, level.x, level.y);

	//pick id
	glUniform1i(UniformLoc::meshid_loc, meshID);
	glUniform1i(UniformLoc::currentid_loc, currentID);
	glBindVertexArray(mesh_data.mVao);
	
	glEnable(GL_DEPTH_TEST);
	if (mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	int n = get_submesh_num();
	for (int i = 0; i < n; i++)
	{
		glUniform3f(UniformLoc::submeshcolor_loc, sub_color.at(i).x, sub_color.at(i).y, sub_color.at(i).z);
		mesh_data.mSubmesh[i].DrawSubmesh();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}



void mesh::initMesh()
{
	mesh_data =  LoadMesh(mesh_name);
}

void mesh::update(fog *f, pointLight *p, directionalLight *dl, transform *t)
{
	this->f = *f;
	this->p = *p;
	this->t = *t;
	this->dl = *dl;
	//this->level = level;
}

void mesh::reloadMeshShader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), tessellation_control_shader.c_str(), tessellation_evaluation_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
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

void mesh::init_shader()
{

	shader = InitShader(vertex_shader.c_str(),tessellation_control_shader.c_str(),tessellation_evaluation_shader.c_str(), geometry_shader.c_str(), fragment_shader.c_str());
}



glm::vec3 mesh::get_world_pos()
{
	return this->t.translate;
}
// static functions
void mesh::test_pos()
{
	std::cout << "test tree's     " << t.translate.x << "    " << t.translate.x << "    " << t.translate.z << std::endl;
}


glm::mat4 mesh::get_M()
{
	return M;
}

void mesh::update_light(directionalLight *dl)
{
	//direction light
	glUseProgram(shader);
	glUniform3f(UniformLoc::dir_loc, dl->dir.x, dl->dir.y, dl->dir.z);
	glUniform1f(UniformLoc::dka_loc, dl->Ka);
	glUniform1f(UniformLoc::dkd_loc, dl->Kd);
	glUniform3f(UniformLoc::dLa_loc, dl->La.x, dl->La.y, dl->La.z);
	glUniform3f(UniformLoc::dLd_loc, dl->Ld.x, dl->Ld.y, dl->Ld.z);
}

int mesh::get_submesh_num()
{
	return mesh_data.mSubmesh.size();
}