#include "Ocean.h"

void Ocean::init()
{
}

GLuint Ocean::get_h0_k_handle()
{
	return this->h0_k.get_handle();
}

GLuint Ocean::get_h0_minus_k_handle()
{
	return this->h0_minus_k.get_handle();
}

Ocean::Ocean()
{
	this->init();
}

void Ocean::render_h0()
{
}
