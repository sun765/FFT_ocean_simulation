#ifndef  __OCEAN_H__
#define  __OCEAN_H__

#define FFT_DIMENSION 256

#include "CompOutputTexture.h"
#include "ComputeShader.h"

class Ocean
{
public:

	void init();

	GLuint get_h0_k_handle();
	GLuint get_h0_minus_k_handle();

	vector<GLuint> noise_textures;

	Ocean();

private:

	CompOutputTexture h0_k_texture;
	CompOutputTexture h0_minus_k_texture;
	CompOutputTexture butterfly_texture;

	

	ComputeShader h0_shader;
	ComputeShader debug_shader;


	void render_h0();
	void init_shaders();
	void init_textures();
	void render_precompute_textures();
};

#endif 
