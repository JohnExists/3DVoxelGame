#version 330 

out vec4 FragColor;

flat in int face;
in vec3 fragPos;
in vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 cameraPos;
uniform float minFog;

float getBrightness(int blockFace)
{
	switch (blockFace)
	{
		case 0: return 0.80f; // Back
		case 1: return 0.80f; // Front
		case 2: return 0.60f; // Left
		case 3: return 0.60f; // Right
		case 4: return 1.0f; // Top
		case 5: return 0.40f; // Bottom
	}
	return 1.0f;
}

void main()
{
	float fog_maxdist = minFog * 1.4f;
	float fog_mindist = minFog;
	vec4  fog_colour = vec4(0.53, 0.81, 0.92, 1.0);

	float dist = length(fragPos - cameraPos);
	float fog_factor = (fog_maxdist - dist) /(fog_maxdist - fog_mindist);
	fog_factor = clamp(fog_factor, 0.0, 1.0);

	vec3 result = texture(texture1, texCoord).rgb * getBrightness(face);
	vec4 objectColour = vec4(result, texture(texture1, texCoord).a);
	FragColor = mix(fog_colour, objectColour, fog_factor);
}