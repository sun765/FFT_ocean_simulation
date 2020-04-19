#include "screenUI.h"
std::string screenUI::vertex_shader = "Shaders/gui_vs.glsl";
std::string screenUI::fragment_shader = "Shaders/gui_fs.glsl";
GLuint screenUI::shader = -1;



screenUI::screenUI()
{
	init();
}


screenUI::~screenUI()
{
}

screenUI::screenUI(float window_w, float window_h, glm::vec2 wh, glm::vec2 gui_pos, GLuint texture_id)
{
	this->window_wh = glm::vec2(window_w, window_h);
	update(wh, gui_pos);
	this->texture_id = texture_id;
	init();
}

void screenUI::init_vao()
{
	this->vao = create_vao();
}

void screenUI::init_shader()
{
	shader=InitShader(vertex_shader.c_str(), fragment_shader.c_str());
}

void screenUI::init()
{
	init_vao();
}

void screenUI::reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(),  fragment_shader.c_str());
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



GLuint screenUI::create_vbo()
{
	//       clipspace x    y   z          u    v

	// this is wrong !    
	//float data[] = { -1.0,1.0,0.0,        0.0,0.0,
	//				  -1.0,-1.0,0.0,       0.0,1.0,
	//				   1.0,-1.0,0.0,       1.0,1.0,
	//				   1.0,1.0,0.0,        1.0,0.0 };

	float data[] = { -1.0,-1.0,0.0,       0.0,0.0,
					 -1.0,1.0,0.0,        0.0,1.0,
				      1.0,-1.0,0.0,       1.0,0.0,
				      1.0,1.0,0.0,        1.0,1.0 };

	GLuint position_buffer;
	glGenBuffers(1, &position_buffer); //Generate vbo to hold vertex attributes for triangle.
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer); //Specify the buffer where vertex attribute data is stored.
	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

	return position_buffer;// why doing this ? because you are return the vbo object!
}

GLuint screenUI::create_vao()
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

void screenUI::drawUI(glm::vec2 gui_wh, glm::vec2 gui_pos, GLuint texture_id,float strength, float fade_factor)
{
	glUseProgram(this->shader);

	// send pos and width height to shader and do the calculate in the shader
	glUniform1i(UniformLoc::flaretexture_loc, 11);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glUniform2f(UniformLoc::guiwh_loc, gui_wh.x,gui_wh.y);
	glUniform2f(UniformLoc::guipos_loc, gui_pos.x, gui_pos.y);
	glUniform2f(UniformLoc::windowWH_loc, window_wh.x, window_wh.y);
	glUniform1f(UniformLoc::strength_loc,strength);
	glUniform1f(UniformLoc::fadefactor_loc, fade_factor);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(0);
}

void screenUI::update(glm::vec2 gui_wh, glm::vec2 gui_pos)
{
	this->gui_wh = gui_wh;
	this->gui_pos = gui_pos;
}


void screenUI::draw_depth_UI(glm::vec2 gui_wh, glm::vec2 gui_pos)
{
	glUseProgram(this->shader);

	// send pos and width height to shader and do the calculate in the shader
	glUniform1i(UniformLoc::flaretexture_loc, 11);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glUniform2f(UniformLoc::guiwh_loc, gui_wh.x, gui_wh.y);
	glUniform2f(UniformLoc::guipos_loc, gui_pos.x, gui_pos.y);
	glUniform2f(UniformLoc::windowWH_loc, window_wh.x, window_wh.y);
	glUniform1f(UniformLoc::strength_loc, 1.0);
	glUniform1f(UniformLoc::fadefactor_loc, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(0);

}