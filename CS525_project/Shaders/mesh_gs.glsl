#version 430

//Pass-through
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
layout(location = 4) uniform mat4 V;
layout(location = 8) uniform mat4 P;

//layout(location = 0) in vec3 normal_in[]; 
//layout(location = 1) in vec3 fragWPos_in[]; 

layout(location = 3) in float test_color_in[];

layout(location = 0) out vec3 normal; 
layout(location = 1) out vec3 fragWPos; 
layout(location = 2) out vec3 flatNormal; 
layout(location = 3) out float test_color;


vec3 calNormal(vec3 u, vec3 v);
void main()
{
	
	vec3 p0 =  gl_in[0].gl_Position.xyz;
	vec3 p1 =  gl_in[1].gl_Position.xyz;
	vec3 p2 =  gl_in[2].gl_Position.xyz;

	vec3 u = p1 - p0;
	vec3 v = p2 - p0;

		for( int i = 0; i< 3; i++)

		{
		//this is important!
			gl_ClipDistance[0] = gl_in[i].gl_ClipDistance[0];
			//normal = normal_in[i];
			fragWPos = vec3(gl_in[i].gl_Position); 
			flatNormal = calNormal( u, v);
			 test_color=test_color_in[i];
			gl_Position = P * V*gl_in[i].gl_Position;
			EmitVertex();

		}
		EndPrimitive();


}


vec3 calNormal(vec3 u, vec3 v)
{
	return -normalize(cross(u,v));

}