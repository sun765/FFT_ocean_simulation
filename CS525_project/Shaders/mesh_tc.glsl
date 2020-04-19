#version 430
layout(vertices = 3) out; 
layout(location = 82 )uniform vec2 level;
layout(location = 32) uniform vec3 eyeWPos;
layout(location = 0) in vec3 normal[];




patch out vec3 b[10];
patch out vec3 b_c[3];
patch out float test;
float calc_average_dist();

void main ()
{
//Pass-through vertices coming from vertex shader
//In the tessellation control language, gl_InvocationID contains the index of the output patch vertex assigned to the shader invocation. It is assigned an integer value in the range [0, N-1] where N is the number of output patch vertices.
gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

float dist = calc_average_dist();
float attenuation = pow(dist*0.01,1.5);
//test = dist;
//test = level.x;

//gl_TessLevelInner[0] = 0.01*dist;
//gl_TessLevelOuter[0] = 0.01*dist;
//gl_TessLevelOuter[1] = 0.01*dist;
//gl_TessLevelOuter[2] = 0.01*dist;

gl_TessLevelInner[0] = level.x/attenuation;
gl_TessLevelOuter[0] = level.y/attenuation;
gl_TessLevelOuter[1] = level.y/attenuation;
gl_TessLevelOuter[2] = level.y/attenuation;
//gl_TessLevelInner[0] = 10.0;
//gl_TessLevelOuter[0] = 10.0;
//gl_TessLevelOuter[1] = 10.0;
//gl_TessLevelOuter[2] = 10.0;

// all computed in eye space
vec3 P[3];
for(int i = 0; i< 3; i++)
{
	 P[i] = vec3(gl_in[i].gl_Position);
}

for(int i = 0; i< 3; i++)
{
	b[i] = P[i];
}

float w[6];


for( int index = 0; index<3; index++)
{	
	int i = index ;
	int j = (i+1)%3;
	w[2*index] = dot((P[j]-P[i]),normal[i]);
	w[2*index+1] = dot((P[i]-P[j]),normal[j]);
}
float w12 = dot((P[1]-P[0]),normal[0]);
float w21 = dot((P[0]-P[1]),normal[1]);
float w23 = dot((P[2]-P[1]),normal[1]);
float w32 = dot((P[1]-P[2]),normal[2]);
float w31 = dot((P[0]-P[2]),normal[2]);
float w13 = dot((P[2]-P[0]),normal[0]);



	b[3] = (2*P[0]+P[1] -w12*normal[0])/3.0;
	b[4] = (2*P[1]+P[0] -w21*normal[1])/3.0;
	b[5] = (2*P[1]+P[2] -w23*normal[1])/3.0;
	b[6] = (2*P[2]+P[1] -w32*normal[2])/3.0;
	b[7] = (2*P[2]+P[0] -w31*normal[2])/3.0;
	b[8] = (2*P[0]+P[2] -w13*normal[0])/3.0;

	vec3 E =  (b[3]+ b[4]+ b[5]+ b[6]+ b[7]+ b[8])/6.0;
	vec3 V = (P[0]  + P[1]  +P[2]  )/3.0;


	b[9] = E + ((E-V)/2.0);



	b_c[0]  = vec3(1.0, 0.0, 0.0); 
	b_c[1]  = vec3(0.0, 1.0, 0.0); 
	b_c[2]  = vec3(0.0, 0.0, 1.0); 


}

float calc_average_dist()
{

	float sum= 0.0;
	for(int i = 0; i< 3 ; i++)
	{
	//
		float dist = length(eyeWPos-vec3(gl_in[i].gl_Position));
		sum+= dist;
	}
	return sum/3.0;

}



