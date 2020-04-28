#ifndef  __OCEAN_H__
#define  __OCEAN_H__

#define FFT_DIMENSION 256
#define PI 3.1415926
#define G 9.81


#include "CompOutputTexture.h"
#include "ComputeShader.h"

#include <random>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Ocean
{
public:

	void init();
	void render();
	void reconfig(float amplitude, float windspeed, float alignment, glm::vec2& wind_dir);

	GLuint get_h0_r_handle();
	GLuint get_h0_i_handle();
	GLuint get_wkt_handle();
	GLuint get_h0_k_handle();
	GLuint get_h0_minus_k_handle();
	GLuint get_hkt_handle();
	GLuint get_twiddle_handle();
	GLuint get_displacement_handle();
	GLuint get_twiddle_debug_handle();
	GLuint get_ht_handle();
	GLuint get_ifft_buffer_handle();
	GLuint get_dxy_handle();
	GLuint get_debug_input_handle();
	GLuint get_debug_output_handle();

	float get_amplitude();
	float get_windspeed();
	float get_alignment();
	glm::vec2 get_wind_dir();

	vector<GLuint> noise_textures;

	Ocean();

private:

	int ocean_dimension = 256;           
	float amplitude = 0.45f * 1e-3f;  // A
	float windspeed = 6.5f;     
	float alignment = 2.0;            // |k * w|^ (alignment);
	float patch_size = 20.0f;
	glm::vec2 wind_dir = glm::vec2(1.0, 1.0);

	vector<float> h0data_r;      // real part of h0k
	vector<float> h0data_i;      // imagine part of h0k
	vector<float> wkdata;        // sqrt(k * g);

	// not used , just for debugging
	CompOutputTexture h0_k_texture;     
	CompOutputTexture h0_minus_k_texture;

	GLuint h0_array_texture_r;
	GLuint h0_array_texture_i;
	GLuint wkt_texture;

	CompOutputTexture hkt_texture;
	CompOutputTexture butterfly_texture;
	CompOutputTexture twiddle_factor_texture;
	CompOutputTexture displacement_texture;
	CompOutputTexture twiddle_debug_texture;
	CompOutputTexture IFFT_buffer_texture;
	CompOutputTexture ht_texture;
	CompOutputTexture dxy_texture;
	CompOutputTexture debug_input_texture;
	CompOutputTexture debug_output_texture;


	ComputeShader h0_shader;
	ComputeShader hkt_shader;
	ComputeShader debug_shader;
	ComputeShader twiddle_factor_shader;
	ComputeShader displacement_shader;
	ComputeShader twiddle_debug_shader;
	ComputeShader IFFT_shader;


	void render_hkt();
	void render_h0();   // this is just for debugging now
	
	void render_twiddle_factor();
	void render_displacement();
	void render_twiddle_debug();
	void render_precompute_textures();

	
	// return value is the pingpong stage
	int  compute_IFFT_helper(CompOutputTexture * input_texture, CompOutputTexture* output_texture);
	void compute_IFFT();
	void compute_IFFT(CompOutputTexture& input_texture);

	void init_shaders();
	void init_textures();

	// helper function to reverse the bit
	int reverse_bit(int i, int bit_num);
	// helper function to compute phillis component of spectrum
	float Phillips(const glm::vec2& k, const glm::vec2& w, float V, float A);
	// precompute h0 data into vectors
	void compute_h0();
	// bind float vector to a ssbo 
	void bind_ssbo_float(vector<float>& data, int loc);
	void bind_ssbo_int(vector<int>& data, int loc);


	
};

#endif 
