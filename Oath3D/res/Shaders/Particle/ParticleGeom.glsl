#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 vs_color[];
out vec3 fs_color;
out vec2 fs_txCoords;

uniform mat4 ProjMatrix;

const vec2 offsets[4] = { vec2(0.f, 0.f), vec2(1.f, 0.f), vec2(0.f, 1.f), vec2(1.f, 1.f) };

void main() {

	fs_color = vs_color[0];
	
	for (int i = 0; i < 4; i++) {
		fs_txCoords = offsets[i];
		vec4 pos = gl_in[0].gl_Position;
		pos.xy += .5f * (offsets[i] - vec2(0.5f));
		gl_Position = ProjMatrix * pos;
		EmitVertex();
	}
}