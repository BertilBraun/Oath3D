#include "Common/Common.h"
#include "BasicMesh.h"

void Oath::Comp::BasicMesh::bind() const {
	GLCall(glBindVertexArray(VAO));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
}

void Oath::Comp::BasicMesh::Draw() const {
	GLCall(glDrawArrays(GL_TRIANGLES, 0, size));
}

void Oath::Comp::BasicMesh::release() {
	GLCall(glDeleteBuffers(1, &VBO));
	GLCall(glDeleteVertexArrays(1, &VAO));
	VAO = VBO = 0;
}