#ifndef GLSL_SHADER
#define GLSL_SHADER


constexpr const char* DEF_VERTEX_SHADER = "#version 330 core\n	"
"layout(location = 0) in vec3 aPos;								"
"layout(location = 1) in int aBlockFace;						"
"layout(location = 2) in vec2 aTexCoord;						"
"																"
"uniform mat4 model;											"
"uniform mat4 view;												"
"uniform mat4 projection;										"
"																"
"flat out int face;												"
"out vec3 fragPos;												"
"out vec2 texCoord;												"
"																"
"void main()													"
"{																"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);	"
"	texCoord = aTexCoord;										"
"	face = aBlockFace;											"
"	fragPos = vec3(model * vec4(aPos, 1.0));					"
"}";

constexpr const char* DEF_FRAG_SHADER = "#version 330 core\n	"
"																\n"
"out vec4 FragColor;											\n"
"																\n"
"flat in int face;												\n"
"in vec3 fragPos;												\n"
"in vec2 texCoord;												\n"
"																\n"
"uniform sampler2D texture1;									\n"
"uniform vec3 cameraPos;										\n"
"uniform float minFog;										\n"
"																\n"
"float getBrightness(int blockFace)								\n"
"{																\n"
"	switch (blockFace)											\n"
"	{															\n"
"		case 0: return 0.80f; // Back							\n"
"		case 1: return 0.80f; // Front							\n"
"		case 2: return 0.60f; // Left							\n"
"		case 3: return 0.60f; // Right							\n"
"		case 4: return 1.0f; // Top								\n"
"		case 5: return 0.40f; // Bottom							\n"
"	}															\n"
"	return 1.0f;												\n"
"}																\n"
"void main()													\n"
"{																\n"
"	float fog_maxdist = minFog * 1.4f;							\n"
"	float fog_mindist = minFog;									\n"
"	vec4  fog_colour = vec4(0.53, 0.81, 0.92, 1.0);				\n"
"																\n"
"	float dist = length(fragPos - cameraPos);					\n"
"	float fog_factor = (fog_maxdist - dist) /(fog_maxdist - fog_mindist);\n"
"	fog_factor = clamp(fog_factor, 0.0, 1.0);					\n"
"																\n"
"	vec3 result = texture(texture1, texCoord).rgb				\n"
"		* getBrightness(face);									\n"
"	vec4 objectColour = vec4(result, texture(texture1, texCoord).a);\n"
"	FragColor = mix(fog_colour, objectColour, fog_factor);		\n"
"}";

#define TEMP_VERTEX_SHADER "#version 330 core\n					 \
layout(location = 0) in vec3 aPos;								 \
layout(location = 2) in vec2 aTexCoord;							 \
																 \
uniform mat4 model;												 \
uniform mat4 view;												 \
uniform mat4 projection;										 \																 \
																 \
out vec2 texCoord;												 \
																 \
void main()														 \
{																 \
	gl_Position = model * vec4(aPos, 1.0);						 \
	texCoord = aTexCoord;										 \
																 \
}"

#define TEMP_FRAG_SHADER "#version 330 core\n					\
																\
out vec4 FragColor;												\
																\
in vec2 texCoord;												\
																\
uniform sampler2D texture1;										\
																\
void main()														\
{																\
	FragColor = texture(texture1, texCoord);					\
																\
}"

#endif // !GLSL_SHADER
