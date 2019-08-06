
out mat3 TBN;

void calcTBN(vec3 normal, vec3 tangent) {
	
    vec3 N = normalize(normal);
    vec3 T = normalize(tangent);
	//T = normalize(T - dot(T, N) * N);
    //vec3 B = normalize(cross(T, N));
    vec3 B = normalize(cross(normalize(T - dot(T, N) * N), N));
	
    TBN = mat3(T, B, N);
}