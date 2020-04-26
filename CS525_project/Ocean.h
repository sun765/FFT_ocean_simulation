#ifndef  __OCEAN_H__
#define  __OCEAN_H__

#define FFT_DIMENSION 256

#include "CompOutputTexture.h"
#include "ComputeShader.h"

class Ocean
{
public:

	void init();
	void render();

	GLuint get_h0_k_handle();
	GLuint get_h0_minus_k_handle();
	GLuint get_hkt_handle();
	GLuint get_twiddle_handle();
	GLuint get_displacement_handle();
	GLuint get_twiddle_debug_handle();

	vector<GLuint> noise_textures;

	Ocean();

private:

	CompOutputTexture h0_k_texture;
	CompOutputTexture h0_minus_k_texture;
	CompOutputTexture hkt_texture;
	CompOutputTexture butterfly_texture;
	CompOutputTexture twiddle_factor_texture;
	CompOutputTexture displacement_texture;
	CompOutputTexture twiddle_debug_texture;


	ComputeShader h0_shader;
	ComputeShader hkt_shader;
	ComputeShader debug_shader;
	ComputeShader twiddle_factor_shader;
	ComputeShader displacement_shader;
	ComputeShader twiddle_debug_shader;

	void render_hkt();
	void render_h0();
	void render_twiddle_factor();
	void render_displacement();
	void render_twiddle_debug();
	void render_precompute_textures();

	void init_shaders();
	void init_textures();

	// helper function to reverse the bit
	int reverse_bit(int i, int bit_num);
	
};

#endif 
