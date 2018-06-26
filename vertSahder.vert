#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;
uniform mat4 rotate;
//out float gl_ClipDistance[1];

//uniform vec4 ClipPlane;


in vec4 vtxPos;
in vec4 vtxCol;
in vec3 vtxNormal;

out Data{
	vec4 color;
	vec3 norm; 
} Out;

void main(){

	mat4 normalMat = transpose(inverse(view*model));
	vec3 normal = normalize(vtxNormal);
	normal = normalize((normalMat * vec4(normal, 0)).xyz);

	// transform the vertex position
	gl_Position =   projection * view * model * vtxPos;

	//gl_ClipDistance[0] = dot(ModelMatrix * Position, ClipPlane);
	// set the colour

	Out.color = vtxCol;
}