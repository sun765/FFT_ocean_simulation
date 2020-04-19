#version 430

//Pass-through
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
layout(location = 0) uniform mat4 M;
layout(location = 4) uniform mat4 V;
layout(location = 8) uniform mat4 P;
layout(location = 30) uniform float time;
layout(location = 45) uniform int  shading_mode;
layout(location = 48) uniform float waveLength;
layout(location = 49) uniform float waveAmplitude;

layout(location = 0) in vec4 ClipCoord_in[];
layout(location = 1) in vec2 texCoord_in[];
layout(location = 2) in vec3 fragWPos_in[];
layout(location = 4) in vec3 pos_in[];
layout(location = 5) in vec3 fragEPos_in[];
layout(location = 6) in vec3 depthEPos_in[];



layout(location = 0) out vec4 ClipCoord;
layout(location = 1) out vec2 texCoord;
layout(location = 2) out vec3 fragWPos;
layout(location = 3) out vec3 flatNormal;
layout(location = 5) out vec3 fragEPos;
layout(location = 6) out vec3 depthEPos;


vec3 calNormal(vec3 p0, vec3 p1, vec3 p2);
float generateOffset(float x, float z, float val1, float val2);
vec3  applyDistortion(vec3 vertex);


const float PI = 3.1415926;
void main()
{
	
	vec3 p0 =  gl_in[0].gl_Position.xyz;
	vec3 p1 =  gl_in[1].gl_Position.xyz;
	vec3 p2 =  gl_in[2].gl_Position.xyz;

	vec3 distorted_pos[3];

	for( int i = 0; i< 3;i++)
	{
	   distorted_pos[i] = applyDistortion(pos_in[i]);	
	}
	vec3 fn= calNormal(  distorted_pos[0], distorted_pos[1], distorted_pos[2]);

		for( int i = 0; i< 3; i++)

		{

			ClipCoord = P*V*M*ClipCoord_in[i]; 
			texCoord = texCoord_in[i];
			fragWPos = fragWPos_in[i];
			depthEPos = depthEPos_in[i];
			fragEPos =vec3(V*M * vec4( distorted_pos[i],1.0)) ;
			//flatNormal = calNormal( p0,p1,p2);
			flatNormal  =  fn;
			if(shading_mode == 1)
			{
				gl_Position = P*V*M * vec4( distorted_pos[i],1.0);
			}
			else
			{
				gl_Position = P*V*M * gl_in[i].gl_Position;
			}
			EmitVertex();
		}
		EndPrimitive();


}


vec3 calNormal(vec3 p0, vec3 p1, vec3 p2)
{
	vec3 u = p1 - p0;
	vec3 v = p2 - p0;
	return normalize(cross(u,v));

}

float generateOffset(float x, float z, float val1, float val2){
	float radiansX = ((mod(x+z*x*val1, waveLength)/waveLength) + 0.0005*time * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
	float radiansZ = ((mod(val2 * (z*x +x*z), waveLength)/waveLength) + 0.0005*time * 2.0 * mod(x , 2.0) ) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
	float xDistortion = generateOffset(vertex.x, vertex.z, 0.2, 0.1);
	float yDistortion = generateOffset(vertex.x, vertex.z, 0.1, 0.3);
	float zDistortion = generateOffset(vertex.x, vertex.z, 0.15, 0.2);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}
