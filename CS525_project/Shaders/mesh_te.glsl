#version 430            

layout (triangles, fractional_odd_spacing, ccw) in;
patch in vec3 b[10];
patch in float test;
layout(location = 3) out float test_color;
//layout(location = 12 )uniform int pn_on;

//layout(location = 0) in vec3 normal_in[]; 
//layout(location = 1) in vec3 fragWPos_in[]; 
//layout(location = 2) out vec3 pos;

	//eye-space

//layout(location = 30) out vec3 b_coord;





void main()
{
float u = gl_TessCoord[0];
float v = gl_TessCoord[1];
float w = gl_TessCoord[2];
test_color = test;

    //        gl_Position = u*gl_in[0].gl_Position 
    //         +v*gl_in[1].gl_Position
			 //+w*gl_in[2].gl_Position  ;
			
//	else
//	{
	vec3 pos = vec3( b[0]*pow(w,3.0) + b[1]*pow(u,3.0)+b[2]*pow(v,3.0) +
					b[3]*3.0*w*w*u   +b[4]*3.0*w*u*u   +b[8]*3.0*w*w*v   +b[5]*3.0*u*u*v   +b[7]*3.0*w*v*v   +b[6]*3.0*u*v*v+
					+b[9]*6.0*w*u*v);
	gl_Position = vec4(pos,1.0);

	//test = vec3(1.0);


}