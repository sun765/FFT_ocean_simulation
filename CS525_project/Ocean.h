#ifndef  __OCEAN_H__
#define  __OCEAN_H__

#define FFT_DIMENSION 256
#define PI 3.1415926
#define G 9.81


#include "CompOutputTexture.h"
#include "ComputeShader.h"
#include "VertexFragShader.h"
#include "QuadMesh.h"

#include <random>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Ocean
{
public:

	void init();
	void render(glm::mat4& M, glm::mat4& V, glm::mat4& P, glm::vec3& eye_world_pos, GLuint skybox_handle);
	void reconfig(float amplitude, float windspeed, float alignment, glm::vec2& wind_dir, int patch_size);
	void update  (int choppy_on, float choppy_factor, glm::vec3& sun_color, glm::vec3 & sun_dir, glm::vec3& ocean_color, int shading_mode, float height_factor);

	GLuint get_h0_k_handle();
	GLuint get_h0_minus_k_handle();
	GLuint get_hkt_handle();
	GLuint get_xkt_handle();
	GLuint get_zkt_handle();
	GLuint get_twiddle_handle();
	GLuint get_displacement_handle();
	GLuint get_twiddle_debug_handle();
	GLuint get_normal_handle();
	GLuint get_Jacobian_handle();
	GLuint get_ifft_buffer_handle();
	GLuint get_debug_input_handle();
	GLuint get_debug_output_handle();

	float get_amplitude();
	float get_windspeed();
	float get_alignment();
	float get_choppy_factor();

	int   get_choppy_status();
	int   get_patch_size();
	int   get_shading_mode();

	glm::vec2 get_wind_dir();
	glm::vec3 get_ocean_color();
	glm::vec3 get_sun_color();
	glm::vec3 get_sun_dir();

	vector<GLuint> noise_textures;

	Ocean();
	Ocean(int dimension);

private:

	// parameters (pre compute)
	int   ocean_dimension = 256;       
	float amplitude       = 0.45f * 1e-3f;  // A
	float windspeed       = 6.5f;     
	float alignment       = 2.0;            // |k * w|^ (alignment);
	float patch_size      = 20.0f;

	// parameters (real time)
	int   choppy_on = 1;                    // 1 on, 0 off
	float choppy_factor   = 1.3f;           // how choppy the ocean is 
	float height_factor   = 1.0f;


	glm::vec2 wind_dir    = glm::vec2(1.0, 1.0);
	//glm::vec4 ocean_color   = glm::vec4(0.0056f, 0.0194f, 0.0331f, 1);
	//glm::vec4 ocean_color = glm::vec4(0.1812f, 0.4678f, 0.5520f, 1);
	glm::vec4 ocean_color = glm::vec4(4.0/255.0, 16.0 / 255.0, 28.0 /255.0, 1);
	vector<float> h0data_r;      // real part of h0k
	vector<float> h0data_i;      // imagine part of h0k
	vector<float> wkdata;        // sqrt(k * g);

	// shading parameters
	int shading_mode = 0; // 0 ward model, 1 blin phong
	glm::vec3 sun_color = glm::vec3(169.0/255.0, 169.0 / 255.0, 159.0 /255.0);
	glm::vec3 sun_dir   = glm::vec3(0.603, 0.240, -0.761);

	// not used , just for debugging
	CompOutputTexture h0_k_texture;     
	CompOutputTexture h0_minus_k_texture;

	CompOutputTexture hkt_texture;            // frequency value in y  direction     
	CompOutputTexture xkt_texture;            // frequency value in x  direction
	CompOutputTexture zkt_texture;            // frequency value in z  direction
	CompOutputTexture butterfly_texture;
	CompOutputTexture twiddle_factor_texture;
	CompOutputTexture displacement_texture;
	CompOutputTexture twiddle_debug_texture;
	CompOutputTexture IFFT_buffer_texture;
	CompOutputTexture normal_texture;
	CompOutputTexture Jacobian_texture;
	CompOutputTexture debug_input_texture;
	CompOutputTexture debug_output_texture;

	ComputeShader h0_shader;
	ComputeShader hkt_shader;
	ComputeShader debug_shader;
	ComputeShader twiddle_factor_shader;
	ComputeShader displacement_shader;
	ComputeShader twiddle_debug_shader;
	ComputeShader IFFT_shader;
	ComputeShader normal_shader;

	VertexFragShader render_shader;

	QuadMesh ocean_surface;

	void render_hkt();
	void render_h0();   // this is just for debugging now
	
	void render_twiddle_factor();
	void render_displacement();
	void render_normal_map();
	void render_twiddle_debug();
	void render_precompute_textures();

	// return value is the pingpong stage
	int  compute_IFFT_helper(CompOutputTexture * input_texture, CompOutputTexture* output_texture);
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
