

#include "GlobalVariables.h"

void update();
void render_scene(int pass, glm::vec4 plane,camera camera);
void draw_gui();
void reload_shader();
void init_shader();
void init_render_class();
void init_textures();


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

	float aspect_ratio = float(w) / float(h);
	glm::vec3 test1 = main_camera.get_camera_position();


   //clear the screen
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glEnable(GL_CLIP_DISTANCE0);

   ///////////////////////////////////////////////////
   // Begin pass 1: render reflection to texture.
   ///////////////////////////////////////////////////  

   glBindFramebuffer(GL_FRAMEBUFFER, fbo); // Render to FBO.
   glDrawBuffer(GL_COLOR_ATTACHMENT0);
   //bind renderbuffer here
   glBindFramebuffer(GL_FRAMEBUFFER,fbo);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
   glViewport(0, 0, w,h);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glm::vec4 reflection_clip_plane = glm::vec4(0.0,1.0,0.0,-main_water->w.water_height);
   render_scene(1, reflection_clip_plane,reflect_camera);


 ///////////////////////////////////////////////////
// Begin pass 2: render refraction to texture.
/////////////////////////////////////////////////// 
   glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
   glDrawBuffers(2,refraction_buffers);
   //bind renderbuffer here
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

   glViewport(0, 0, w, h);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//std::cout << main_camera.get_py().y << std::endl;
   glm::vec4 refraction_clip_plane = glm::vec4(0.0, -1.0, 0.0, main_water->w.water_height);
   render_scene(2,refraction_clip_plane,main_camera);
   
 
   
 ///////////////////////////////////////////////////
// Begin pass 3: render movable objects to pick texture.
/////////////////////////////////////////////////// 

   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   glDrawBuffer(GL_COLOR_ATTACHMENT2);
   //bind renderbuffer here
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

   glViewport(0, 0, w, h);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // need change
   render_scene(3, glm::vec4(0.0, 1.0, 0.0, 2000.0), main_camera);


   ///////////////////////////////////////////////////
   // Begin pass 5: render scene
   ///////////////////////////////////////////////////
   glBindFramebuffer(GL_FRAMEBUFFER, 0); 
   glDrawBuffer(GL_BACK); // Render to back buffer.
   
   glViewport(0, 0, w, h); //Render to the full window
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   

   render_scene(4,none,main_camera);

   if (!logo_on&&!logo2_on)
   {
	   draw_gui();
   }


   if (recording == true)
   {
      glFinish();

      glReadBuffer(GL_BACK);
      read_frame_to_encode(&rgb, &pixels, w, h);
      encode_frame(rgb);
   }
   
   glutSwapBuffers();
 
}
// glut idle callback.
//This function gets called between frames
void idle()
{
	glutPostRedisplay();

    time_ms = float(glutGet(GLUT_ELAPSED_TIME));
    time_sec = 0.001f*time_ms;
	time_per_frame = time_ms - last_time;
	last_time = time_ms;
	if (time_sec > 15.0)
	{
		logo_on = false;
	}

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

void initOpenGl()
{
	std::cout << "start init" << std::endl;
    //initialize window width and height.
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	aspect_ratio = float(w) / float(h);
	std::cout << aspect_ratio << std::endl;
	P = glm::perspective(3.141592f / 4.0f, aspect_ratio, near_clip, far_clip);
    //Initialize glew so that new OpenGL function names can be used
    glewInit();

    //load data
	std::cout << "start init load" << std::endl;
    load();


   float aspect_ratio = float(w) / float(h);
   texture_width = w;
   texture_height = h;

   //Initialize mesh shader (static)
   std::cout << "start init shader" << std::endl;
   init_shader();
   std::cout << "start init render class" << std::endl;
   init_render_class();
   std::cout << "start init textures" << std::endl;
   init_textures();
  

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
   case 'f':
		/*
	   glm::vec3 pos = mousepicker->getCurrentTerrainPoint();
	   transform this_transform = { pos,
					   glm::vec3(create_scale.at(displaymesh_id)),
					   0.0,
					   1
	   };
	   transform_list.at(displaymesh_id).push_back(this_transform);
	   mesh m = mesh(mesh_filenames[displaymesh_id], 0, &f, &p, &dl, &this_transform, tess_level);
	   mesh_list.at(displaymesh_id).push_back(m);
	   std::cout << "created" << std::endl;
	   */
	   break;
   }
   if (key == (char)13)
   {
	   logo2_on = false;
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

	const int h = glutGet(GLUT_WINDOW_HEIGHT);
	GLubyte buffer[4];
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, h - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	currentId = int(buffer[0]) / 255;
	terrainpos = glm::vec3(float(buffer[0]) / float(256), float(buffer[1]) / float(256), float(buffer[2]) / float(256));
	for (int i = 0; i < 3; i++)
	{
		terrainpos[i] = terrainpos[i] * 2.0 - 1.0;
	}
	glm::vec4 ClipPos = glm::vec4(terrainpos, 1.0);
	glm::mat4 V = main_camera.get_view_matrice();
	P = glm::perspective(3.141592f / 4.0f, aspect_ratio, near_clip, far_clip);
	glm::vec4 worldPos = glm::inverse(P)*ClipPos;
	worldPos = glm::inverse(V)*worldPos;
	/*
	mousepicker->update(x, y,&main_camera,main_terrain);
	glm::vec3 a = mousepicker->getCurrentRay();
	glm::vec3 test_pos = mousepicker->getCurrentTerrainPoint();
	display_trasnform =  {
						test_pos,
						glm::vec3(create_scale.at(displaymesh_id)),
						0.0,
						1
	};
	*/

}

void mouse(int button, int state, int x, int y)
{
	// read the color into the buffer 
	ImGui_ImplGlut_MouseButtonCallback(button, state);
}

int main (int argc, char **argv)
{
   //Configure initial window state using freeglut
//#if _DEBUG
//
//	glutInitContextFlags(GLUT_DEBUG);
//#endif
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

   initOpenGl();

   ImGui_ImplGlut_Init(); // initialize the imgui system

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

void render_scene(int pass, glm::vec4 plane, camera camera)
{
	
   if (logo_on)
   {
	   logo->drawUI(glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), logo_texture_id, 1.0, 1.0);
   } 
   else if (logo2_on)
   {
	   logo2->drawUI(glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), logo2_texture_id, 1.0, 1.0);
   }

   else
   {
	   glm::mat4 R = glm::rotate(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	   //glm::mat4 T = glm::translate(glm::vec3(-main_terrain->get_terrain_wh().x / 2.0, 0.0, -main_terrain->get_terrain_wh().y / 2.0));
	   glm::mat4 S = glm::scale(glm::vec3(10.0));
	   //glm::mat4 M = R * S * T;
	   glm::mat4 V = camera.get_view_matrice();
	   
	   //glm::mat4 T2 = glm::translate(glm::vec3(-main_terrain->get_terrain_wh().x / 2.0, main_water->w.water_height, -main_terrain->get_terrain_wh().y / 2.0));
	   //glm::mat4 M2 = R *  T2*S;
	   glm::vec3 hard_code = glm::vec3(-1500, 0, -1500);
	   //std::cout << "x: " << hard_code.x << " y: " << hard_code.y << " z: " << hard_code.z << std::endl;
	   glm::mat4 M_water = R * glm::translate(glm::vec3(hard_code.x, main_water->w.water_height, hard_code.z))*S;
	   glm::vec3 cameraPos = main_camera.get_camera_position();
	   glm::mat4 M_tree = R * glm::scale(glm::vec3(3.0)) *glm::translate(glm::vec3(0.0));
	   glm::mat4 M_tree2 = R * glm::scale(glm::vec3(3.0)) *glm::translate(glm::vec3(-30.0, 10.0, 30.0));



	   //render reflection and refraction textures
	   if (pass == 1 || pass == 2)
	   {
		   if (skybox_on)

		   {
			   main_sky->draw_sky(skybox_id, V, P, pass);
		   }
		   else
		   {
			   tsky->drawUI(glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), tsky_texture_id, 1.0, 1.0);
		   }

		  
		   main_sun->draw_sun(P);
		   //main_terrain->draw_terrain(shading_mode, M, V, P, texture_id, pass, plane, cameraPos);

		   for (int i = 0; i < 6; i++)
		   {
			   int mesh_number = mesh_list.at(i).size();
			   for (int j = 0; j < mesh_number; j++)
			   {
				   mesh_list.at(i).at(j).draw_mesh(shading_mode, lastID, V, P, plane, cameraPos, pass, &dl, &f, mesh_line_mode, tess_level, mesh_colors.at(i), time_sec,&transform_list.at(i).at(j));
			   }
			   
		   }

	   }
	   //render terrain pos to textures
	   else if (pass == 3)
	   {
		     //main_terrain->draw_terrain(shading_mode, M, V, P, texture_id, pass, plane, cameraPos);
		   
	   }
	   // render to dynamic cube map
	   else if (pass == 5)
	   {
		       //main_terrain->draw_terrain(shading_mode, M, V, P, texture_id, pass, plane, cameraPos);
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
				   tsky->drawUI(glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), tsky_texture_id, 1.0, 1.0);
			   }

			   main_sun->draw_sun(P);
			   


			   if (terrain_linemode)
			   {
				   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			   }
			   //main_terrain->draw_terrain(shading_mode, M, V, P, texture_id, pass, plane, cameraPos);
			   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			   int mesh_types = 6;
			   for (int i = 0; i < mesh_types; i++)
			   {
				   int mesh_number = mesh_list.at(i).size();
				   for (int j = 0; j < mesh_number; j++)
				   {
					   mesh_list.at(i).at(j).draw_mesh(shading_mode, lastID, V, P, plane, cameraPos, pass, &dl, &f, mesh_line_mode, tess_level, mesh_colors.at(i), time_sec, &transform_list.at(i).at(j));
				   }
			   }

			 
			   if (water_linemode)
			   {
				   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			   }
			   main_water->draw_water(shading_mode, test, M_water, V, P, waterTexture_id, pass, float(time_ms), cameraPos);
			   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			   main_lensflare->conditional_render();
			   


		   }
	   }

   }

	
		
}

void update()
{
	display_list.at(displaymesh_id).update(&f, &p, &dl, &display_trasnform);
	main_water->update(clip_distance,&p, &f, &wP, &qP,&tP);
	//main_terrain->update(clip_distance, &tP, &qP, &f, &dl);
	main_sun->update(&main_camera, &sP);
	main_lensflare->update(main_sun->get_screen_coord(P), &sP,&lP);

	int mesh_nums;

}

void reload_shader()
{


		main_water->reload_shader();
		terrain::reload_shader();
		main_sky->reload_shader();
		mesh::reloadMeshShader();
		screenUI::reload_shader();
		sun::reload_shader();
		layeredRenderingMesh::reload_shader();
		

}

void init_shader()
{
	terrain::initShader();
	mesh::init_shader();
	screenUI::init_shader();
	sun::init_shader();
	lensFlare::init_window_wh(w, h);
	layeredRenderingMesh::init_shader();
}

void draw_gui()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Raleway-Medium.ttf", 14.0f);

	ImGui_ImplGlut_NewFrame();
	myGUIStyle();
	ImGui::Begin("GUI", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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

		if (recording == false)
		{
			if (ImGui::Button("Start Recording"))
			{
				const int w = glutGet(GLUT_WINDOW_WIDTH);
				const int h = glutGet(GLUT_WINDOW_HEIGHT);
				recording = true;
				start_encoding(video_filename, w, h); //Uses ffmpeg
			}

		}
		else
		{
			if (ImGui::Button("Stop Recording"))
			{
				recording = false;
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
	
	// shading mode
	// water parameters
	if (ImGui::CollapsingHeader("tessellation level"))
	{
		ImGui::SliderFloat("inner", &tess_level.x, 0.0f, +10.0f);
		ImGui::SliderFloat("outer", &tess_level.y, 0.0f, +10.0f);
		ImGui::Checkbox("mesh line mode", &mesh_line_mode);
	}

	if (ImGui::CollapsingHeader("shading mode"))
	{
		ImGui::RadioButton("normal shading", &shading_mode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("flat shading", &shading_mode, 1);	
	}

	if (ImGui::CollapsingHeader("entities"))
	{
		if (ImGui::TreeNode("create"))
		{
			ImGui::Checkbox("creating mode", &creating_mode);
			for (int i = 0; i < 6; i++)
			{
				char title[10];
				std::string a = mesh_names[i];
				strcpy(title, a.c_str());
				ImGui::RadioButton(title, &displaymesh_id, i);
				char scale_title[20];
				std::string b = "new "+mesh_names[i]+"scale";
				strcpy(scale_title, b.c_str());
				ImGui::SliderFloat(scale_title, &create_scale.at(i), 0.0f, 1.0f);
				//ImGui::SameLine();
			}
			ImGui::TreePop();
		}
		// how to go to the next line?
		if (ImGui::TreeNode("edit"))
		{
			for (int i = 0; i < 6; i++)
			{
				char title[10];
				std::string a = mesh_names[i];
				strcpy(title, a.c_str());
				
				if (ImGui::TreeNode(&title[0]))
				{
					char class_title[20];
					std::string b = mesh_names[i] + " class";
					strcpy(class_title, b.c_str());
					//-----------------------------mesh class color edit-------------------------------//
					if (ImGui::TreeNode(&class_title[0]) )
					{

							int n =display_list.at(i).get_submesh_num();
							for (int j = 0; j < n; j++)
							{
								char title[7];
								std::string c = "color" + std::to_string(j);
								strcpy(title, c.c_str());
								ImGui::ColorEdit3(&title[0], glm::value_ptr(mesh_colors.at(i).at(j)));
							}
							ImGui::TreePop();
					}
					char list_title[15];
					std::string d = mesh_names[i] + " list";
					strcpy(list_title, d.c_str());
					//-----------------------------loop through all the meshes-------------------------------//
					if (ImGui::TreeNode(&list_title[0]))
					{

						int n = mesh_list.at(i).size();

						for (int j = 0; j < n; j++)
						{
							char mesh_title[10];
							std::string e = mesh_names[i] +std::to_string(j);
							strcpy(mesh_title, e.c_str());

							ImGui::Text(&mesh_title[0]);
							ImGui::SameLine();

							char title[8][20];
							std::string title_string[] = { "hightlight","transform","scale","rotate","rotate axis","X","Y","Z" };
							for (int k = 0; k < 8; k++)
							{
								std::string name_string = title_string[k] + mesh_names[i] + std::to_string(j);
								strcpy(title[k], name_string.c_str());
							}
							
							int index = 0;
							ImGui::Checkbox(title[index], &mesh_list.at(i).at(j).hight_light);
							index++;
							ImGui::SliderFloat3(title[index], glm::value_ptr(transform_list.at(i).at(j).translate), -200.0f, 200.0f);
							index++;
							ImGui::SliderFloat3(title[index], glm::value_ptr(transform_list.at(i).at(j).scale), 0.0f, 1.0f);
							index++;
							ImGui::SliderFloat(title[index], &transform_list.at(i).at(j).rotateAngle, 0.0f, +5.0f);
							index++;
							ImGui::SliderInt3(title[index], transform_list.at(i).at(j).rotateAxis, -1, 1);
							index++;

							ImGui::RadioButton(title[index], &transform_list.at(i).at(j).rotateAxis[0], 0);
							index++;
							ImGui::SameLine();
							ImGui::RadioButton(title[index], &transform_list.at(i).at(j).rotateAxis[1], 1);
							index++;
							ImGui::SameLine();
							ImGui::RadioButton(title[index], &transform_list.at(i).at(j).rotateAxis[2], 2);
							index++;


							std::string f = "delete " + mesh_names[i]  +std::to_string(j);
							char delete_title[20];
							strcpy(delete_title, f.c_str());
							if (ImGui::Button(&delete_title[0]))
							{
								mesh_list.at(i).erase(mesh_list.at(i).begin() + j);
								transform_list.at(i).erase(transform_list.at(i).begin() + j);
								break;

							}

						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}

			}
			ImGui::TreePop();
		}
		


	}
	if (ImGui::CollapsingHeader("sky"))
	{
		ImGui::Checkbox("skybox", &skybox_on);
	}
	if (ImGui::CollapsingHeader("water"))
	{
		ImGui::Checkbox("line mode", &terrain_linemode);
		ImGui::SliderFloat3("water color", glm::value_ptr(wP.water_color), 0.0f, 1.0f);
		ImGui::SliderFloat("water mix", &wP.water_color_mix, 0.0f, +1.0f);
		ImGui::SliderFloat("water height", &wP.water_height, -10.0f, +10.0f);
		ImGui::SliderFloat("wave strength", &wP.wave_strength, 0.0f, +0.1f);
		ImGui::SliderFloat("wave speed", &wP.wave_speed, 0.0f, +0.01f);
		ImGui::SliderFloat("wave length", &wP.wave_length, 0.0f, +1.01f);
		ImGui::SliderFloat("wave amplitude", &wP.wave_amplitude, 0.0f, +1.0f);
		ImGui::SliderFloat("edge softness", &wP.edge_softness, 0.001f, +10.0f);
		ImGui::SliderFloat("glitch offset", &wP.glitch_offset, 0.0f, +10.0f);
		ImGui::SliderFloat("min blue", &wP.minBlue, 0.0f, +1.0f);
		ImGui::SliderFloat("max blue", &wP.maxBlue, 0.0f, +1.0f);
		ImGui::SliderFloat("murky depth", &wP.murkyDepth, 0.1f, +20.0f);



		ImGui::Image((void*)waterTexture_id[0], ImVec2(128.0f, 128.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
		ImGui::SameLine();
		ImGui::Image((void*)waterTexture_id[1], ImVec2(128.0f, 128.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
		ImGui::Image((void*)waterTexture_id[4], ImVec2(128.0f, 128.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
		if (ImGui::Button("test"))
		{
			test = !test;
		}

		ImGui::SliderFloat("shiness", &wP.shineness, 0.01f, +30.0f);


		if (ImGui::Button("water line mode"))
		{
			water_linemode = !water_linemode;
		}
	}

	if (ImGui::CollapsingHeader("terrain"))
	{

		ImGui::Image((void*)terrainpostexture_id, ImVec2(128.0f, 128.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
		ImGui::ColorEdit3("peak color", glm::value_ptr(tP.peak_color));
		ImGui::ColorEdit3("high color", glm::value_ptr(tP.highpart_color));
		ImGui::ColorEdit3("middle color", glm::value_ptr(tP.middlepart_color));
		ImGui::ColorEdit3("under water color", glm::value_ptr(tP.underwater_color));
		ImGui::SliderFloat("peak_height", &tP.peak_height, -1.0f, +1.0f);
		ImGui::SliderFloat("high_height", &tP.high_height, -1.0f, +1.0f);
		ImGui::SliderFloat("middle_height", &tP.middle_height, -1.0f, +1.0f);
		ImGui::SliderFloat("under_height", &tP.under_height, -1.0f, +1.0f);

	}

	// fog parameters
	if (ImGui::CollapsingHeader("fog"))
	{
		ImGui::SliderFloat("density", &f.density, 0.0f, +5.0f);
		ImGui::SliderFloat("gradient", &f.gradient, 0.0f, +20.0f);
		ImGui::ColorEdit3("fog_color", glm::value_ptr(f.fog_color));
	}

	// transofrm parameters

	

	// sun and lens flare
	if (ImGui::CollapsingHeader("sun"))
	{
		ImGui::SliderFloat3("sun pos", glm::value_ptr(sP.sun_pos), -1000.0f, 1000.0f);
		ImGui::SliderFloat2("sun scale", glm::value_ptr(sP.sun_scale), 0, 500.0f);
		ImGui::SliderFloat("flare strength", &lP.strength, 0.0f, +1.0f);
		ImGui::SliderFloat("flare fade factor", &lP.fadefactor, 0.0f, +1.0f);
		int nums = main_lensflare->get_nums();
		for (int i = 0; i < nums; i++)
		{
			// why one more?
			char title[7];
			std::string a = "flare" + std::to_string(i);
			strcpy(title, a.c_str());

			ImGui::Image((void*)main_lensflare->get_texture_id(i), ImVec2(128.0f, 128.0f), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
			ImGui::SliderFloat(&title[0], &lP.scale[i], 0.0f, +500.0f);
		}


	}


	if (ImGui::CollapsingHeader("lighting"))
	{
		if (ImGui::CollapsingHeader("point light"))
		{
			ImGui::SliderFloat("Ka", &p.Ka, 0.0f, +1.0f);
			ImGui::SliderFloat3("La", glm::value_ptr(p.La), 0.0f, 1.0f);
			ImGui::SliderFloat("Kd", &p.Kd, 0.0f, +1.0f);
			ImGui::SliderFloat3("Ld", glm::value_ptr(p.Ld), 0.0f, 1.0f);
			ImGui::SliderFloat("Ks", &p.Ks, 0.0f, +1.0f);
			ImGui::SliderFloat3("Ls", glm::value_ptr(p.Ls), 0.0f, 1.0f);
			ImGui::SliderFloat3("LightPos", glm::value_ptr(p.lightPos), -100.0f, 100.0f);

		}

		if (ImGui::CollapsingHeader("direction light"))
		{
			ImGui::SliderFloat(" d Ka", &dl.Ka, 0.0f, +1.0f);
			ImGui::SliderFloat3("d La", glm::value_ptr(dl.La), 0.0f, 1.0f);
			ImGui::SliderFloat(" d Kd", &dl.Kd, 0.0f, +1.0f);
			ImGui::SliderFloat3("d Ld", glm::value_ptr(dl.Ld), 0.0f, 1.0f);
			ImGui::SliderFloat3("direction", glm::value_ptr(dl.dir), -1.0f, 1.0f);

		}
	}

	ImGui::End();

	ImGui::Render();


}

void init_render_class()
{
	std::cout << "start init cameras" << std::endl;
	//Initialize camearas
	main_camera = camera(mainC);
	reflect_camera = camera(mainC);
	depth_camera = camera(mainC);
	//testui = new screenUI(w,h,,glm::vec2( 0.5*w, 0.5*h ),0);
	logo = new screenUI(w, h, glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), logo_texture_id);
	logo2 = new screenUI(w, h, glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), logo2_texture_id);
	tsky = new screenUI(w, h, glm::vec2(w, h), glm::vec2(0.5*w, 0.5*h), tsky_texture_id);
	main_sun = new sun(glm::vec2(w, h), &sP, &main_camera);
	main_lensflare = new lensFlare(main_sun->get_screen_coord(P), &sP, &lP);
	//mousepicker = new mousePicking(w,h,&main_camera,main_terrain->get_scale(), P);


	std::cout << "start init water" << std::endl;
	// intit all the render class
	main_water = new water(clip_distance, &p, &f, &wP, &qP, depth_camera, &tP);
	std::cout << "start init terrain, skybox" << std::endl;
	//main_terrain = new terrain(clip_distance, &tP, &qP,  &dl, &f, depth_camera);
	std::cout << "start init skybox" << std::endl;
	main_sky = new skybox();
	
	std::cout << "start init transform" << std::endl;
	for (int i = 0; i < 6; i++)
	{
		transform t = {
						glm::vec3(-20.0,0.0,20.0),
						glm::vec3(0.05),
						0.0,
						1
		};
		std::vector<transform> list1;
		list1.push_back(t);
		transform_list.push_back(list1);


		std::vector<glm::vec3> list2;
		for (int j = 0; j < 10; j++)
		{
			glm::vec3 color = glm::vec3(0.5);
			list2.push_back(color);
		}
		mesh_colors.push_back(list2);
		create_scale.push_back(0.05);
	}


	std::cout << "start init meshes" << std::endl;
	//initialize all meshes;
	for (int i = 0; i < 6; i++)
	{
		std::vector<mesh> list1;
		
		int mesh_number = transform_list.at(i).size();
		for (int j = 0; j < mesh_number; j++)
		{
			mesh new_mesh = mesh(mesh_filenames[i], i, &f, &p, &dl, &transform_list.at(i).at(j), tess_level);
			list1.push_back(new_mesh);
		}
		mesh_list.push_back(list1);

	}

	std::cout << "start init display meshes" << std::endl;
	//initialize all display meshes;
	int types = 6;
	for (int i = 0; i < types; i++)
	{
		transform temp = {
						glm::vec3(-20.0,0.0,20.0),
						glm::vec3(0.05),
						0.0,
						1
		};
			mesh new_mesh = mesh(mesh_filenames[i], 0, &f, &p, &dl, &temp, tess_level);
			display_list.push_back(new_mesh);
	}
	

}

void init_textures() {


	for (int i = 0; i < texture_num; i++)
	{
		texture_id[i] = LoadTexture(texture_name[i].c_str());
	}

	skybox_id = LoadCube(skybox_name);
	//skybox_id = crystal->cubemap_id;

	//load dudv map and normal map

	waterTexture_id[2] = LoadTexture(DuDv.c_str());
	waterTexture_id[3] = LoadTexture(water_normal.c_str());

	//load logo texture;
	logo_texture_id = LoadTexture(logo_texture_name.c_str());
	logo2_texture_id = LoadTexture(logo2_texture_name.c_str());
	tsky_texture_id = LoadTexture(tsky_texture_name.c_str());

	//-----------------------------------------------------------------fbo initialization----------------------------------------------------------------------------------//
	//Create a texture object and set initial wrapping and filtering state


	for (int i = 0; i < 2; i++)
	{
		glGenTextures(1, &waterTexture_id[i]);
		glBindTexture(GL_TEXTURE_2D, waterTexture_id[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	glGenTextures(1, &terrainpostexture_id);
	glBindTexture(GL_TEXTURE_2D, terrainpostexture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// this looks ugly ! need to be changed
	glGenTextures(1, &waterTexture_id[4]);
	glBindTexture(GL_TEXTURE_2D, waterTexture_id[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* setup framebuffer with cubemap attached */;


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
}
//}


