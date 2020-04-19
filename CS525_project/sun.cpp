#include "sun.h"
std::string sun::vertex_shader = "Shaders/sun_vs.glsl";
std::string sun::fragment_shader = "Shaders/sun_fs.glsl";
GLuint sun::shader = -1;


sun::sun()
{
}

sun::sun(glm::vec2 window_wh, sunP *sP,camera *cam)
{
	this->window_wh = window_wh;
	update(cam, sP);
	init_vao();
	init_texture();
}


sun::~sun()
{
}

void sun::init_vao()
{
	this->vao = create_vao();
}

void sun::init_shader()
{
	shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());
}

void sun::init()
{
	init_vao();
}



GLuint sun::create_vbo()
{
	//       clipspace x    y   z          u    v

	// this is wrong !    
	//float data[] = { -1.0,1.0,0.0,        0.0,0.0,
	//				  -1.0,-1.0,0.0,       0.0,1.0,
	//				   1.0,-1.0,0.0,       1.0,1.0,
	//				   1.0,1.0,0.0,        1.0,0.0 };

	float data[] = { -1.0,-1.0,0.0,       0.0,1.0,
					 -1.0,1.0,0.0,        0.0,0.0,
					  1.0,-1.0,0.0,       1.0,1.0,
					  1.0,1.0,0.0,        1.0,0.0 };

	GLuint position_buffer;
	glGenBuffers(1, &position_buffer); //Generate vbo to hold vertex attributes for triangle.
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); //Specify the buffer where vertex attribute data is stored.
	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

	return position_buffer;// why doing this ? because you are return the vbo object!
}

GLuint sun::create_vao()
{
	GLuint vbo = create_vbo();
	GLuint vao;

	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);

	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(vao);


	glEnableVertexAttribArray(AttribLoc::Pos_loc); //Enable the position attribute.
	glEnableVertexAttribArray(AttribLoc::TexCoord_loc);

	glVertexAttribPointer(AttribLoc::Pos_loc, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
	glVertexAttribPointer(AttribLoc::TexCoord_loc, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));// change 
	glBindVertexArray(0);

	return vao;
}

void sun::update(camera *cam, sunP* sP)
{
	this->cam = *cam;
	this->sP = *sP;
}

void sun::draw_sun(glm::mat4 P)
{
	glUseProgram(shader);

	// send pos and width height to shader and do the calculate in the shader
	//glm::vec3 r, u;
	//r = cam.get_camera_right();
	//u = cam.get_camera_up();
	//r = glm::normalize(r);
	//u = glm::normalize(u);

	glm::vec2 screen_coord = get_screen_coord(P);
	//std::cout << screen_coord.x << "    " << screen_coord.y << std::endl;

	//glUniform3f(UniformLoc::camRight_loc, r.x,r.y,r.z);
	//glUniform3f(UniformLoc::camUp_loc, u.x,u.y,u.z);
	//

	glUniform2f(UniformLoc::sunPos_loc, screen_coord.x, screen_coord.y);
	glUniform2f(UniformLoc::sunScale_loc, sP.sun_scale.x, sP.sun_scale.y);
	glUniform2f(UniformLoc::windowWH_loc,window_wh.x, window_wh.y);

	//glUniformMatrix4fv(UniformLoc::PVM_loc, 1, false, glm::value_ptr(P*V*M));

	glUniform1i(UniformLoc::suntexture_loc, 13);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(0);
}

void sun::reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());
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

glm::vec2 sun::get_screen_coord(glm::mat4 P)
{
	glm::mat4 V = cam.get_view_matrice();
	//std::cout << cam.get_camera_front().x << "    " << cam.get_camera_front().y << "    " << cam.get_camera_front().z << std::endl;
	glm::mat4 M = glm::translate(sP.sun_pos);
	//std::cout << sP.sun_pos.x << "    " << sP.sun_pos.y << std::endl;
	glm::vec4 screen_coord = P*V*M*glm::vec4(0.0,0.0,0.0, 1.0);
	//std::cout << screen_coord.x << "    " << screen_coord.y << std::endl;
	return glm::vec2(screen_coord.x/ screen_coord.w, screen_coord.y/ screen_coord.w);

}
 
void sun::test(glm::mat4 P)
{
	//std::cout << get_screen_coord(P).x << "     " << get_screen_coord(P).y << std::endl;
}

void sun::init_texture()
{
	texture_name = "Textures/sun3.png";
	texture_id = LoadTexture(texture_name.c_str());
}
