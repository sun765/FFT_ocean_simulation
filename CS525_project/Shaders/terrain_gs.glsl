 #version 430

//Pass-through

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout(location = 1) in vec2 tex_coord_in[]; 
layout(location = 2) in vec3 pos_in[];	
layout(location = 3) in vec3 normal_in[];
layout(location = 4) in vec3 fragEPos_in[]; //tangent space to eye space
layout(location = 5) in float fogDistance_in[]; //tangent space to eye space
layout(location = 6) in vec3 fragWPos_in[]; //tangent space to eye space
layout(location = 9) in vec3 depthEPos_in[];
layout(location = 10) in flat vec3 diffuse_color_in[];
layout(location = 11) in vec3 fragClipPos_in[];

layout(location = 25) uniform vec4 plane;
layout(location = 8) uniform mat4 P;
layout(location = 24) uniform int pass;



layout(location = 1) out vec2 tex_coord; 
layout(location = 3) out vec3 normal;
layout(location = 4) out vec3 fragEPos; //tangent space to eye space
layout(location = 5) out float fogDistance; //tangent space to eye space
layout(location = 6) out vec3 fragWPos; //tangent space to eye space
layout(location = 7) out vec3 flatNormal; //tangent space to eye space
layout(location = 8) out float test; //tangent space to eye space
layout(location = 9) out vec3 depthEPos;
layout(location = 10) out flat vec3 diffuse_color;
layout(location = 11) out vec3 fragClipPos;


vec3 calNormal(vec3 u, vec3 v);
vec3 calflat_color(vec3 u, vec3 v);
void main()
{


	vec3 p0 =  gl_in[0].gl_Position.xyz;
	vec3 p1 =  gl_in[1].gl_Position.xyz;
	vec3 p2 =  gl_in[2].gl_Position.xyz;

	vec3 u = p1 - p0;
	vec3 v = p2 - p0;
	vec3 fn = calNormal( u,  v);

		for( int i = 0; i< 3; i++)

		{
			gl_ClipDistance[0] = gl_in[i].gl_ClipDistance[0];
			test = gl_ClipDistance[0];
			tex_coord = tex_coord_in[i];
			// how to cal per primitive normal?
			normal= normal_in[i];
			flatNormal = fn;
			depthEPos = depthEPos_in[i];
			//flatNormal = p0;
			fragEPos = fragEPos_in[i];
			fogDistance = fogDistance_in[i];
			fragWPos = fragWPos_in[i];
			diffuse_color = diffuse_color_in[i];
			fragClipPos =fragClipPos_in[i];
			
			gl_Position = P * gl_in[i].gl_Position;
			EmitVertex();

		}

			EndPrimitive();


}


vec3 calNormal(vec3 u, vec3 v)
{
	return normalize(cross(u,v));

}