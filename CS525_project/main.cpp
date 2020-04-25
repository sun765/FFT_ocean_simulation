

#include "GlobalVariables.h"

void update();
void render_scene(int pass, glm::vec4 plane,camera camera);
void draw_gui();
void reload_shader();
void init_shader();
void init_render_class();
void init_textures();
void init_imgui();
void init_2D_texture(GLuint texture_id, int width, int height);


void display()
{

	update();
	float d = main_camera.get_camera_position().y - main_water->w.water_height;
	glm::vec3 origin_pos = main_camera.get_camera_position();
	glm::vec3 new_pos = glm::vec3(main_camera.get_camera_position().x, main_water->w.water_height - d, main_camera.get_camera_position().z);
	reflect_camera.set_camera_position(new_pos);
	glm::vec2 main_yp = main_camera.get_yp();
	glm::vec2 reflect_yp = glm::vec2(main_yp.x, -main_yp.y);
	reflect_camera.set_yp(reflect_yp);
	glm::vec4 none = glm::vec4(0.0, 1.0, 0.0, 2000.0);

	float aspect_ratio = float(window_width) / float(window_height);

   glEnable(GL_CLIP_DISTANCE0);

   ///////////////////////////////////////////////////
   // Begin pass 1: render scene
   ///////////////////////////////////////////////////
   glBindFramebuffer(GL_FRAMEBUFFER, 0); 
   glDrawBuffer(GL_BACK); // Render to back buffer.
   
   glViewport(0, 0, window_width, window_height); //Render to the full window
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   

   render_scene(4,none,main_camera);

   draw_gui();



   if (is_recording == true)
   {
      glFinish();

      glReadBuffer(GL_BACK);
      read_frame_to_encode(&rgb, &pixels, window_width, window_height);
      encode_frame(rgb);
   }
   
   glutSwapBuffers();
 
}

// glut idle callback. This function gets called between frames
void idle()
{
	glutPostRedisplay();

    time_ms = float(glutGet(GLUT_ELAPSED_TIME));
    time_sec = 0.001f*time_ms;
	time_per_frame = time_ms - last_time;
	last_time = time_ms;

}

// Display info about the OpenGL implementation provided by the graphics driver.
// Your version should be > 4.0 for CGT 521 
void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void init_application()
{
    //initialize window width and height.
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);
	aspect_ratio = float(window_width) / float(window_height);
	std::cout << aspect_ratio << std::endl;
	P = glm::perspective(3.141592f / 4.0f, aspect_ratio, near_clip, far_clip);

    //Initialize glew so that new OpenGL function names can be used
    glewInit();

    //load data
    load();

   float aspect_ratio = float(window_width) / float(window_height);
   texture_width = window_width;
   texture_height = window_height;

   //Initialize shader, render class and textures
   init_shader();
   init_render_class();
   init_textures();
   init_imgui();

   glEnable(GL_DEPTH_TEST);

}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
   if (!pause)
   {
	   main_camera.move_camera(key, 1.0f);
   }

   switch (key)
   {
   case ' ':
	   pause = !pause;
	   break;
   }

   glm::vec3 position = main_camera.get_camera_position();
   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
}

void special_keyboard(int key, int x, int y)
{


}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void pasive_motion(int x, int y)
{

	ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
	if (!pause)
	{
		float dx, dy;
		dx = x - previous_campos.x;
		dy = y - previous_campos.y;
		previous_campos = glm::vec2(x, y);
		main_camera.turn_camera(-dx, -dy);
	}
	/*

	const int h = glutGet(GLUT_WINDOW_HEIGHT);
	GLubyte buffer[4];
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, h - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	current_id = int(buffer[0]) / 255;
	terrain_pos = glm::vec3(float(buffer[0]) / float(256), float(buffer[1]) / float(256), float(buffer[2]) / float(256));
	for (int i = 0; i < 3; i++)
	{
		terrain_pos[i] = terrain_pos[i] * 2.0 - 1.0;
	}
	glm::vec4 ClipPos = glm::vec4(terrain_pos, 1.0);
	glm::mat4 V = main_camera.get_view_matrice();
	P = glm::perspective(3.141592f / 4.0f, aspect_ratio, near_clip, far_clip);
	glm::vec4 worldPos = glm::inverse(P)*ClipPos;
	worldPos = glm::inverse(V)*worldPos;
	*/
}

void mouse(int button, int state, int x, int y)
{
	ImGui_ImplGlut_MouseButtonCallback(button, state);
}

int  main (int argc, char **argv)
{
   //Configure initial window state using freeglut
   //#if _DEBUG
   //	glutInitContextFlags(GLUT_DEBUG);
   //#endif
   // init glut
   glutInitContextVersion(4, 3);

   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (1280, 720);
   int win = glutCreateWindow ("Ocean rendering");
   glutSetCursor(GLUT_CURSOR_NONE);

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutSpecialFunc(special_keyboard);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(pasive_motion);
   glutIdleFunc(idle);

   // init application
   init_application();

   ImGui_ImplGlut_Init(); // initialize the imgui system

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

void render_scene(int pass, glm::vec4 plane, camera camera)
{
	
	   glm::mat4 R = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	   glm::mat4 S = glm::scale(glm::vec3(10.0));
	   glm::mat4 V = camera.get_view_matrice();
	   

	   glm::vec3 hard_code = glm::vec3(-1500, 0, -1500);
	   glm::mat4 M_water = R * glm::translate(glm::vec3(hard_code.x, main_water->w.water_height, hard_code.z))*S;
	   glm::vec3 cameraPos = main_camera.get_camera_position();
	   glm::mat4 M_tree = R * glm::scale(glm::vec3(3.0)) *glm::translate(glm::vec3(0.0));
	   glm::mat4 M_tree2 = R * glm::scale(glm::vec3(3.0)) *glm::translate(glm::vec3(-30.0, 10.0, 30.0));



	   //render reflection and refraction textures
	   if (pass == 1 || pass == 2)
	   {		

	   }

	   //render scene
	   else if (pass == 4)
	   {
		   if (renderScene)
		   {
			   
			   if (skybox_on)

			   {
				   main_sky->draw_sky(skybox_id, V, P, pass);
			   }
			   else
			   {
				   tsky->drawUI(glm::vec2(window_width, window_height), glm::vec2(0.5*window_width, 0.5*window_height), tsky_texture_id, 1.0, 1.0);
			   }

			   main_sun->draw_sun(P);
			  

			   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			 
			   if (water_linemode)
			   {
				   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			   }
			   main_water->draw_water(shading_mode, test, M_water, V, P, waterTexture_id, pass, float(time_ms), cameraPos);
			   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			   
		   }
	   }

		
}

void update()
{

	main_water->update(clip_distance,&p, &f, &wP, &qP,&tP);
	main_sun->update(&main_camera, &sP);

}

void reload_shader()
{
	main_water->reload_shader();
	main_sky->reload_shader();
	screenUI::reload_shader();
	sun::reload_shader();
}

void init_shader()
{
	water::init_shader();
	screenUI::init_shader();
	sun::init_shader();
	//comp_shader = new ComputeShader("Shaders/test.comp");
	
}

void draw_gui()
{

	ImGui_ImplGlut_NewFrame();
	myGUIStyle();
	ImGui::Begin("Ocean Parameters", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Image((void*)ocean->get_h0_k_handle(), ImVec2(256.0f, 256.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
	ImGui::Image((void*)ocean->get_h0_minus_k_handle(), ImVec2(256.0f, 256.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
	for (int i = 0; i < 4; i++) {
		ImGui::Image((void*)ocean->noise_textures[i], ImVec2(256.0f, 256.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
	}

	if (ImGui::CollapsingHeader("debug")) {
		ImGui::Image((void*)comp_texture->get_handle(), ImVec2(128.0f, 128.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
	}

	if (ImGui::CollapsingHeader("general"))
	{
		const int filename_len = 256;
		static char video_filename[filename_len] = "capture.mp4";
		ImGui::InputText("Video filename", video_filename, filename_len);
		ImGui::SliderFloat("time per frame(ms)", &time_per_frame, 0.0f, +1000.0f);
		if (ImGui::Button("Reload_Shader"))
		{
			reload_shader();// does it do anything ?
		}

		if (is_recording == false)
		{
			if (ImGui::Button("Start Recording"))
			{
				const int w = glutGet(GLUT_WINDOW_WIDTH);
				const int h = glutGet(GLUT_WINDOW_HEIGHT);
				is_recording = true;
				start_encoding(video_filename, w, h); //Uses ffmpeg
			}

		}
		else
		{
			if (ImGui::Button("Stop Recording"))
			{
				is_recording = false;
				finish_encoding(); //Uses ffmpeg
			}
		}

		//create a slider to change the angle variables
		ImGui::SliderFloat("View angle", &angle, -3.141592f, +3.141592f);


		if (ImGui::Button("renderscene"))
		{
			renderScene = !renderScene;
		}
		if (ImGui::Button("save"))
		{
			bool state = save();
			if (!state)
			{
				std::cout << "save failed!"<<std::endl;
			}
			else
			{
				std::cout << "save success!" << std::endl;
			}
		}
	}
	
	if (ImGui::CollapsingHeader("sky"))
	{
		ImGui::Checkbox("skybox", &skybox_on);
	}


	ImGui::End();

	ImGui::Render();


}

void init_render_class()
{
	//Initialize camearas
	main_camera = camera(mainC);
	reflect_camera = camera(mainC);
	depth_camera = camera(mainC);
	
	// intit all the render class
	main_water = new water(clip_distance, &p, &f, &wP, &qP, depth_camera, &tP);
	main_sun = new sun(glm::vec2(window_width, window_height), &sP, &main_camera);
	main_sky = new skybox();
	tsky = new screenUI(window_width, window_height, glm::vec2(window_width, window_height), glm::vec2(0.5 * window_width, 0.5 * window_height), tsky_texture_id);
	ocean = new Ocean();


}

void init_textures() {

	int texture_num = sizeof(texture_ids) / sizeof(texture_ids[0]);

	for (int i = 0; i < texture_num; i++)
	{
		texture_ids[i] = LoadTexture(texture_name[i].c_str());
	}

	skybox_id = LoadCube(skybox_name);

	//load dudv map and normal map
	waterTexture_id[2] = LoadTexture(dudv.c_str());
	waterTexture_id[3] = LoadTexture(water_normal.c_str());

	//load logo texture;
	tsky_texture_id = LoadTexture(tsky_texture_name.c_str());

	//-----------------------------------------------------------------fbo initialization----------------------------------------------------------------------------------//
	//Create a texture object and set initial wrapping and filtering state


	for (int i = 0; i < 4; i++)
	{
		if (i == 3)
			continue;
		init_2D_texture(waterTexture_id[i], window_width, window_height);
	}

	//Create the framebuffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);  //why need to bind it here?
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//attach the texture we just created to color attachment 1
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, waterTexture_id[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, waterTexture_id[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, terrainpostexture_id, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, waterTexture_id[4], 0);

	//unbind the fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//test compute shader
	comp_texture = new CompOutputTexture(FFT_DIMENSION, FFT_DIMENSION, GL_RGBA32F);

}

void init_2D_texture(GLuint texture_id, int width, int height)
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// debug later
void init_imgui() {
	//ImGui::GetIO().Fonts->AddFontFromFileTTF("Raleway-Medium.ttf", 14.0f);
}



