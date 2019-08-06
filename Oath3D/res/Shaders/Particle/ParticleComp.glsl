#version 430

layout(local_size_x = 64) in;
layout(std140, binding = 0) restrict buffer Pos {
	vec4 position[];
};
layout(std140, binding = 1) restrict buffer Vel {
	vec4 velocity[];
};

uniform float dt;
uniform bool attractorActive;
uniform vec3 attractorPosition;

void main() {
	const uint idx = gl_GlobalInvocationID.x;
	velocity[idx] *= 1 - .99f * dt;
	if (attractorActive) {
		float dist = distance(attractorPosition, position[idx].xyz);
		float acc = 10000.f / max(1.f, 0.01f * pow(dist, 1.5f));
		velocity[idx] += vec4(normalize(attractorPosition - position[idx].xyz), 0.f) * acc * dt;
	}
	position[idx] += velocity[idx] * dt;
}