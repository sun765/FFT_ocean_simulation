#ifndef  __OCEAN_H__
#define  __OCEAN_H__

#include "CompOutputTexture.h"
#include "ComputeShader.h"

class Ocean
{
public:

	void init();

	GLuint get_h0_k_handle();
	GLuint get_h0_minus_k_handle();

	Ocean();

private:

	CompOutputTexture h0_k;
	CompOutputTexture h0_minus_k;
	CompOutputTexture butterfly;

	ComputeShader h0_shader;

	void render_h0();
};

#endif 
