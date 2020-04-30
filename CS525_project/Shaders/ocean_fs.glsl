  
#version 430
#define PI 3.1415926535897932384626433832795

layout (binding = 0) uniform samplerCube envmap;
layout (binding = 1, rgba32f) readonly  uniform image2D normal_map;

out vec4 fragcolor;    

in vec2 tex_coord;
in vec3 view_dir;
in vec3 test_color;
in vec3 normal;
in float J;

uniform vec4 ambient_color;

uniform vec3 sun_color;
uniform vec3 sun_dir;

void main(void)
{

	// calculate thingies
	vec4 normal_j = imageLoad(normal_map, ivec2(tex_coord));

	vec3 n = normalize(normal);
	vec3 v = normalize(view_dir);
	vec3 l = reflect(-v, n);

	float F0 = 0.020018673;
	float F = F0 + (1.0 - F0) * pow(1.0 - dot(n, l), 5.0);

	//vec3 refl = vec3(0.0,0.0,1.0);
	vec3 refl = texture(envmap, l).rgb;
	refl = vec3(0.0);

	// tweaked from ARM/Mali's sample
	float turbulence = max(1.6 - 0.05 *J, 0.0);
	float color_mod = 1.0 + 3.0 * smoothstep(1.2, 1.8, turbulence);


	// some additional specular (Ward model)
	const float rho = 0.3;
	const float ax = 0.2;
	const float ay = 0.1;

	vec3 h = sun_dir + v;
	vec3 x = cross(sun_dir, n);
	vec3 y = cross(x, n);

	float mult = ((1.0/ (PI * 4.0)) * rho / (ax * ay * sqrt(max(1e-5, dot(sun_dir, n) * dot(v, n)))));
	float hdotx = dot(h, x) / ax;
	float hdoty = dot(h, y) / ay;
	float hdotn = dot(h, n);

	float spec = mult * exp(-((hdotx * hdotx) + (hdoty * hdoty)) / (hdotn * hdotn));

	/*
	// modified Blinn-Phong model (cheaper)
	float spec = pow(clamp(dot(sundir, l), 0.0, 1.0), 400.0);
	*/

	//spec = pow(clamp(dot(sun_dir, l), 0.0, 1.0), 400.0);

	fragcolor = vec4(mix(ambient_color.xyz, refl * color_mod, F) + sun_color * spec, 1.0);
	


	fragcolor = vec4(mix(ambient_color.xyz, refl * color_mod, F), 1.0);

	fragcolor = vec4(normal,  1.0);
}

