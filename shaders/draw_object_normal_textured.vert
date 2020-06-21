#version 450

layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout(binding = 1, std430) buffer Lights {
	Light lights[];
};

layout(binding = 2, std140) uniform Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} object;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;
layout(location = 3) out mat3 fs_TBN;

void main()
{
	fs_position = vec3(object.model_matrix * vec4(position, 1.0));

	vec3 N = normalize(mat3(object.model_matrix) * normal);
    vec3 T = normalize(mat3(object.model_matrix) * tangent);
    vec3 B = normalize(mat3(object.model_matrix) * bitangent);

	fs_TBN = transpose(mat3(T, B, N ));

	fs_normal = transpose(inverse(mat3(object.model_matrix))) * normal;
	fs_texture_coordinate = texture_coordinate;

    gl_Position = camera.projection * camera.view * object.model_matrix * vec4(position, 1.0);
}
