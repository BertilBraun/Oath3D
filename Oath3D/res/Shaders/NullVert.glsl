#version 330

in vec4 VertPosition;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;


void main() {

	gl_Position = ProjMatrix * ViewMatrix * ModelMatrix * VertPosition;		
}
